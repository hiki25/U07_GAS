#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CGameplayInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_API ICGameplayInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
};
