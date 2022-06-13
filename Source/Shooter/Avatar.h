#pragma once

#include "BaseCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

UCLASS()
class SHOOTER_API AAvatar : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AAvatar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* TopPersonCamera;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
		TSubclassOf<class UScoreHUD> PlayerScoreHUDClass;

	UPROPERTY()
		class UScoreHUD* PlayerScoreHUD;

	void UpdateScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float MaxScore;
	float PlayerScore;
	float EnemyScore;

private:

	void GoAheadBack(float amount);

	void GoRightLeft(float amount);

	void Yaw(float amount);

	void Fire();

	FVector CamFwd;

	FVector CamRht;

	float RotationSensitivity;

};
