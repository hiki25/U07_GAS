#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class GAS_API ACAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;
	
};
