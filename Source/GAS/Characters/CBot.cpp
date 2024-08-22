#include "CBot.h"
#include "Components/CAttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "UI/CWorldWidget.h"

ACBot::ACBot()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensinComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";
}

void ACBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ACBot::OnSeePawn);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACBot::OnHealthChanged);
}

void ACBot::OnSeePawn(APawn* Pawn)
{
	 AAIController* AIC = Cast<AAIController>(GetController());
	 if (ensure(AIC))
	 {
		SetTargetActor(Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Gat Cha",nullptr,FColor::Black,3.f,true);;
	
	 }
}

void ACBot::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		if (InstigatorActor != this)
		{
		SetTargetActor(InstigatorActor);
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);

		if (HealthBarWidget == nullptr)
		{
			HealthBarWidget= CreateWidget<UCWorldWidget>(GetWorld(), HealthBarWidgetClass);
			if (HealthBarWidget)
			{
			HealthBarWidget->AttachToActor = this;
			HealthBarWidget->AddToViewport();
			}
		}

		if (NewHealth <= 0.f)
		{
			AAIController* AIC = GetController<AAIController>();
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			SetLifeSpan(10.f);

		}
	}
}

void ACBot::SetTargetActor(AActor* NewTarget)
{

	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
	AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}
