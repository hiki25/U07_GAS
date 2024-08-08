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
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
