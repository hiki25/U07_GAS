#include "CPickupBase_AddAction.h"
#include "Components/CActionComponent.h"
#include "Actions/CAction.h"

void ACPickupBase_AddAction::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	UCActionComponent* ActionComp = Cast<UCActionComponent>(InstigatorPawn->GetComponentByClass(UCActionComponent::StaticClass()));
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			FString msg = FString::Printf(TEXT("%s is already Learned"), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, msg);
			return;
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldown();
	}
}