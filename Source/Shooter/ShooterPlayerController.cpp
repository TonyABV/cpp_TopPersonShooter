
#include "ShooterPlayerController.h"
#include "Avatar.h"
#include "MenuHUD.h"


void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AShooterPlayerController::OpenMenu);
	}
}

void AShooterPlayerController::OpenMenu()
{
	if (AMenuHUD* MenuHUD = Cast<AMenuHUD>(GetHUD()))
	{
		MenuHUD->ShowMenu();
	}
}