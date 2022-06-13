#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "ScoreHUD.generated.h"

UCLASS(Abstract)
class SHOOTER_API UScoreHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* PlayerScoreBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* EnemyScoreBar;

	void SetPlayerScore(float CurrentScore, float MaxScore);

	void SetEnemyScore(float CurrentScore, float MaxScore);

};
