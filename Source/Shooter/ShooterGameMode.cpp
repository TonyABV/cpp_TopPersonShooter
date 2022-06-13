
#include "ShooterGameMode.h"
#include "Avatar.h"

#include "MenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AShooterGameMode::AShooterGameMode(): Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/BP_Avatar"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerControllerClass = AShooterPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();

}

void AShooterGameMode::RestartGameplay(bool Won)
{
	if (Won)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AShooterGameMode::ResetLevel, 3.f);
	}
	else
	{
		ResetLevel();
	}
}

void AShooterGameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TopShooterWorld");
}
