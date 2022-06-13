#pragma once

#include "BaseCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

UCLASS(config=Game)
class AShooterProjectile : public AActor
{
	GENERATED_BODY()

public:

	AShooterProjectile();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = Proectile)
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* MovementSet;

	virtual void BeginPlay() override;
};

