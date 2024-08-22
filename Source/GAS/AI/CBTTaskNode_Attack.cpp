#include "CBTTaskNode_Attack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CAttributeComponent.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
	MaxSpread = 2.f;
}

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
				if (!UCAttributeComponent::IsActorAlive(TargetActor))
				{
					return EBTNodeResult::Failed;
				}

				FVector MuzzleLocation = BotCharacter->GetMesh()->GetSocketLocation("Muzzle_Front");
				FVector Directrion = TargetActor->GetActorLocation() - MuzzleLocation;

				FRotator RotationToTargetActor = Directrion.Rotation();
				RotationToTargetActor.Pitch += FMath::RandRange(0.f,MaxSpread);
				RotationToTargetActor.Yaw += FMath::RandRange(-MaxSpread,MaxSpread);

				FActorSpawnParameters Params;
				Params.Instigator = BotCharacter;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, RotationToTargetActor, Params);
				return NewProjectile ? (EBTNodeResult::Succeeded) : (EBTNodeResult::Failed) ;
			}
		}

		
	}
	return  EBTNodeResult::Failed;
}