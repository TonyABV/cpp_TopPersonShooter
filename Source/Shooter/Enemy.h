#pragma once

#include "BaseCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemy();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
		class UAIPerceptionComponent* AIPerComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		FRotator EnemyRotation;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		FVector BaseLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		float MovementSpeed;

	UFUNCTION()
		void OnSensed(const TArray<AActor*>& UpdatedActors);

	void SetNewRotation(FVector TargetPosition, FVector CurrentPosition);

private:

	void Fire();

	bool BackToBaseLocation;
	FVector NewLocation;
	float DistSquared;

	float FireDistance;
	float MinRange;
	float LastAtackTime;
	float Cooldown;

};
