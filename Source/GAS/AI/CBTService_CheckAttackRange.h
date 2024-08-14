#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckAttackRange.generated.h"

UCLASS()
class GAS_API UCBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		FBlackboardKeySelector bInRangeKey;
};
