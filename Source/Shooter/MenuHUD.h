#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

UCLASS()
class SHOOTER_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	TSharedPtr<class SMainMenu> MenuWidget;
	TSharedPtr<class SWidget> MenuWidgetContainer;

	virtual void BeginPlay() override;

public:

	void ShowMenu();
	void RemoveMenu();

};
