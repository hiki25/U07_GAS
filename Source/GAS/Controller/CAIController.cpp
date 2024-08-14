#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this,0);

	if (PlayerPawn)
	{
		//GetBlackboardComponent()->SetValueAsVector("MoveToLocation",PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor",PlayerPawn);

	}

}
