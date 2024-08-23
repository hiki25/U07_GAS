#include "CBTTaskNode_Heal.h"
#include "AIController.h"
#include "Components/CAttributeComponent.h"

EBTNodeResult::Type UCBTTaskNode_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(AIPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetMaxHealth());
	}

	return EBTNodeResult::Succeeded;
}