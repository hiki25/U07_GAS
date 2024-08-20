#include "CBot.h"
#include "Components/CAttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

ACBot::ACBot()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensinComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ACBot::OnSeePawn);

}

void ACBot::OnSeePawn(APawn* Pawn)
{
	 AAIController* AIC = Cast<AAIController>(GetController());
	 if (ensure(AIC))
	 {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor",Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Gat Cha",nullptr,FColor::Black,3.f,true);;
	
	 }
}