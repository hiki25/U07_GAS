#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckHealth.generated.h"

UCLASS()
class GAS_API UCBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_CheckHealth();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector bLowHealthKey;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMax = 1.00, ClampMin = 0.00))
		float LowHealthFraction;
	
};
