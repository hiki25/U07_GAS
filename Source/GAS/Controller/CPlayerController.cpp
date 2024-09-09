#include "CPlayerController.h"
#include "Blueprint/UserWidget.h"

void ACPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Escape", EInputEvent::IE_Pressed,this,&ACPlayerController::ToggleGameMenu);
}

void ACPlayerController::ToggleGameMenu()
{
	if (GameWidget && GameWidget->IsInViewport())
	{
		GameWidget->RemoveFromParent();
		GameWidget = nullptr;

		this->bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	GameWidget = CreateWidget<UUserWidget>(this, GameWidgetClass);
	if (GameWidget)
	{
		GameWidget->AddToViewport(100);
		this->bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ACPlayerController::BeginPlayingState()
{
	BlueprintBegindState();
}

void ACPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
	//Todo. WB_Credit에서 변경된 PS(NULL) 인지 아닌지를 잘 받아오는지 확인
}


