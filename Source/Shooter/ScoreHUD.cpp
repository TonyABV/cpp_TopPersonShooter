
#include "ScoreHUD.h"

#include "Components/ProgressBar.h"

void UScoreHUD::SetPlayerScore(float CurrentScore, float MaxScore)
{
	if (PlayerScoreBar)
	{
		PlayerScoreBar->SetPercent(CurrentScore / MaxScore);
	}
}

void UScoreHUD::SetEnemyScore( float CurrentScore, float MaxScore)
{
	if (EnemyScoreBar)
	{
		EnemyScoreBar->SetPercent(CurrentScore / MaxScore);
	}
}