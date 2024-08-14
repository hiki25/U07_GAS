#include "CBTTaskNode_Attack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"

EBTNodeResult::Type  UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (ensure(AIC))
	{
		ACharacter* BotCharacter = Cast<ACharacter>(AIC->GetPawn());
		if (!BotCharacter)
		{
			return EBTNodeResult::Failed;
		}

		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (ensure(BB))
		{
			AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
			if (TargetActor)
			{
				FVector MuzzleLocation = BotCharacter->GetMesh()->GetSocketLocation("Muzzle_Front");
				FVector Directrion = MuzzleLocation - TargetActor->GetActorLocation();

				FRotator ProjectileRotation = Directrion.Rotation();

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, ProjectileRotation, Params);
			}
		}

		
	}

}