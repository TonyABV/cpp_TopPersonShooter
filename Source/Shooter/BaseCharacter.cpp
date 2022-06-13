
#include "BaseCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(40.0f, 95.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	BodyMesh->AddRelativeLocation(FVector(0.f, 0.f, -90.f));
	BodyMesh->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
	BodyMesh->bCastDynamicShadow = false;
	BodyMesh->CastShadow = false;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6));

	GunOffset = FVector(100.f, 0.f, 0.f);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GunMesh->AttachToComponent(BodyMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("middle_01_rSocket"));

	GunMesh->AddRelativeRotation(FRotator(0.f, 155.f, 0.f));
	GunMesh->AddRelativeLocation(FVector(GunXOffset, GunYOffset, GunZOffset));

	World = GetWorld();

}
