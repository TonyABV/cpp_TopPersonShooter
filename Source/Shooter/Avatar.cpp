
#include "Avatar.h"
#include "ShooterProjectile.h"
#include "ScoreHUD.h"
#include "ShooterPlayerController.h"
#include "ShooterGameMode.h"

#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AAvatar::AAvatar()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraPos = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraPos"));
	CameraPos->SetupAttachment(RootComponent);
	CameraPos->SetUsingAbsoluteRotation(true);
	CameraPos->TargetArmLength = 2100;
	CameraPos->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraPos->bDoCollisionTest = false;

	TopPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Top Person Camera"));
	TopPersonCamera->bUsePawnControlRotation = false;
	TopPersonCamera->SetupAttachment(CameraPos, USpringArmComponent::SocketName);


	PlayerScoreHUDClass = nullptr;
	PlayerScoreHUD = nullptr;

	MaxScore = 10.f;
	PlayerScore = 0.f;
	EnemyScore = 0.f;

	RotationSensitivity = 120.f;
}

void AAvatar::UpdateScore()
{
	PlayerScoreHUD->SetPlayerScore(PlayerScore, MaxScore);
	PlayerScoreHUD->SetEnemyScore(EnemyScore, MaxScore);

	if (PlayerScore == MaxScore || EnemyScore == MaxScore) {
		AShooterGameMode* GameMode = Cast<AShooterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode) {
			GameMode->RestartGameplay(PlayerScore > EnemyScore);
		}
	}
}

void AAvatar::BeginPlay()
{
	Super::BeginPlay();
	
	FVector fwd = TopPersonCamera->GetForwardVector();
	CamFwd.X = -fwd.Z; 
	CamFwd.Y = fwd.Y;
	CamFwd.Z = -fwd.X;

	CamRht = TopPersonCamera->GetRightVector();

	if (IsLocallyControlled() && PlayerScoreHUDClass)
	{
		APlayerController* FPC = GetController<AShooterPlayerController>();
		check(FPC);
		PlayerScoreHUD = CreateWidget<UScoreHUD>(FPC, PlayerScoreHUDClass);
		check(PlayerScoreHUD);
		PlayerScoreHUD->AddToPlayerScreen();
		UpdateScore();
	}
}

void AAvatar::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerScoreHUD)
	{
		PlayerScoreHUD->RemoveFromParent();
		PlayerScoreHUD = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Ahead/Back", this, &AAvatar::GoAheadBack);

	InputComponent->BindAxis("Right/Left", this, &AAvatar::AAvatar::GoRightLeft);

	InputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AAvatar::Fire);
}

void AAvatar::GoAheadBack(float amount)
{
	if (Controller && amount != 0)
	{
		AddMovementInput(CamFwd, amount);
	}
}

void AAvatar::GoRightLeft(float amount)
{
	if (Controller && amount != 0)
	{
		AddMovementInput(CamRht, amount);
	}
}

void AAvatar::Yaw(float amount)
{
	if (Controller && amount != 0) 
	{
		AddControllerYawInput(RotationSensitivity * amount * GetWorld()->GetDeltaSeconds());
	}
}

void AAvatar::Fire()
{
	if (Projectile != NULL && World != NULL)
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
	}
}

