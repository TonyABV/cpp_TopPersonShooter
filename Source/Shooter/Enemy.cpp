
#include "Enemy.h"
#include "Avatar.h"
#include "ShooterProjectile.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 1600.f;
	SightConfig->LoseSightRadius = 1700.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f);

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 400.0f;

	DistSquared = BIG_NUMBER;

	FireDistance = 1100.f;
	MinRange = 300.f;
	Cooldown = 0.4f;
	LastAtackTime = 0.f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	BaseLocation = this->GetActorLocation();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;

		if (BackToBaseLocation)
		{
			if ((NewLocation - BaseLocation).SizeSquared2D() < DistSquared)
			{
				DistSquared = (NewLocation - BaseLocation).SizeSquared2D();
			}
			else
			{
				CurrentVelocity = FVector::ZeroVector;
				DistSquared = BIG_NUMBER;
				BackToBaseLocation = false;
				
				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}

		SetActorLocation(NewLocation);
	}

	LastAtackTime += DeltaTime;
}

void AEnemy::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	for (const auto& Actor : UpdatedActors) {
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(Actor, Info);
		
		if (Info.LastSensedStimuli[0].WasSuccessfullySensed()) 
		{
			FVector playerPos = Actor->GetActorLocation();
			FVector toPlayer = playerPos - GetActorLocation();
			float DistanceToPlayer = toPlayer.Size();

			if (DistanceToPlayer > MinRange)
			{
				FVector dir = Actor->GetActorLocation() - GetActorLocation();
				dir.Z = 0.f;

				CurrentVelocity = dir.GetSafeNormal() * MovementSpeed;
			}
			else
			{
				CurrentVelocity = FVector::ZeroVector;
			}

			SetNewRotation(Actor->GetActorLocation(), GetActorLocation());

			if(LastAtackTime > Cooldown && DistanceToPlayer <= FireDistance)
			{
				SetNewRotation(Actor->GetActorLocation(), GetActorLocation());
				Fire();
			}
		}
		else
		{
			FVector dir = BaseLocation - GetActorLocation();
			dir.Z = 0.f;
			
			if(dir.SizeSquared2D() > 1.f)
			{
				CurrentVelocity = dir.GetSafeNormal() * MovementSpeed;
				BackToBaseLocation = true;

				SetNewRotation(BaseLocation, GetActorLocation());
			}
		}
	}
	
}

void AEnemy::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.f;
	
	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);
}

void AEnemy::Fire()
{
	if (Projectile)
	{
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		SpawnRotation = GetControlRotation();

		SpawnLacation = ((MuzzleLocation != nullptr) ?
			MuzzleLocation->GetComponentLocation() :
			GetActorLocation() + SpawnRotation.RotateVector(GunOffset));

		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		(World->SpawnActor<AShooterProjectile>(Projectile, SpawnLacation, SpawnRotation, SpawnParam))->SetOwner(this);

		LastAtackTime = 0.f;
	}
}

