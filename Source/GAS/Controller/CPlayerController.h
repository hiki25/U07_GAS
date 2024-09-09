#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class GAS_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetPawn(APawn* InPawn) override;
	virtual void BeginPlayingState() override;
	virtual void OnRep_PlayerState() override;

	void SetupInputComponent() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBegindState();

	UFUNCTION(BlueprintCallable)
	void ToggleGameMenu();

public:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameWidgetClass;

	UPROPERTY()
	UUserWidget* GameWidget;
	
};
