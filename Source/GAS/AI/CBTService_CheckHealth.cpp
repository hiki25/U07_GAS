#include "CBTService_CheckHealth.h"
#include "AIController.h"
#include "Components/CAttributeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"

UCBTService_CheckHealth::UCBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}

void UCBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(AIPawn);
		if (ensure(AttributeComp))
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetMaxHealth()) < LowHealthFraction;

			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			BBComp->SetValueAsBool(bLowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
