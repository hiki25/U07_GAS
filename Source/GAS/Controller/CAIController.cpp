#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree,TEXT("Null BehaviorTree")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
