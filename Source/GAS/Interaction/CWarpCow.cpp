#include "CWarpCow.h"
#include "Components/CActionComponent.h"
#include "Characters/CPlayer.h"

ACWarpCow::ACWarpCow()
{

}

void ACWarpCow::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	ACPlayer* Player = Cast<ACPlayer>(InstigatorPawn);

	UCActionComponent* ActionComp = Player->GetActionComp();
	if(ActionComp)
	{
		ActionComp->AddAction(Player, ActionClass);
		HideAndCooldown();
	}

}
