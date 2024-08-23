#include "CBTService_CheckAttackRange.h"
#include "BehaviorTree/Blackboardcomponent.h"
#include "AIController.h"

UCBTService_CheckAttackRange::UCBTService_CheckAttackRange()
{
	AttackRange = 2000.f;
}

void UCBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (ensure(BB))
	{
		AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* AIC = OwnerComp.GetAIOwner();
			if ensure((AIC))
			{
				APawn* BotPawn = AIC->GetPawn();
				if (ensure(BotPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(),BotPawn->GetActorLocation());
					bool bInRange = Distance < AttackRange;

					bool bLOS = false;
					if (bInRange)
					{

					bLOS = AIC->LineOfSightTo(TargetActor);
					}

					BB->SetValueAsBool(bInRangeKey.SelectedKeyName,bInRange && bLOS);
				}
			}
		}
	}



}
