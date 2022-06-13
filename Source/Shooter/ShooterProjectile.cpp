
#include "ShooterProjectile.h"
#include "Avatar.h"
#include "Enemy.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AShooterProjectile::AShooterProjectile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(5.0f);
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	SphereComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = SphereComp;

	MovementSet = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementSet->UpdatedComponent = SphereComp;
	MovementSet->InitialSpeed = 2000.f;
	MovementSet->MaxSpeed = 2000.f;
	MovementSet->bRotationFollowsVelocity = true;
	MovementSet->bShouldBounce = true;

	MovementSet->Bounciness = 1.0f;
	MovementSet->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.f;
}

void AShooterProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentHit.AddDynamic(this, &AShooterProjectile::OnHit);
}

void AShooterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* Shooter = Cast<AAvatar>(GetOwner());

	if (Shooter == OtherActor) { return; }

	AAvatar* Player = Cast<AAvatar>(OtherActor);

	if (Player) 
	{
		++Player->EnemyScore;
		Player->UpdateScore();

		Destroy();
	}
	else if (Cast<AEnemy>(OtherActor))
	{
		Player = Cast<AAvatar>(GetOwner());
		if (Player)
		{
			if (++Player->PlayerScore == Player->MaxScore)
			{
				OtherActor->Destroy();
			}

			Player->UpdateScore();
		}

		Destroy();
	}
}