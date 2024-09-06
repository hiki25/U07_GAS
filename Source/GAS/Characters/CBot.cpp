#include "CBot.h"
#include "Components/CAttributeComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	ActionComp = CreateDefaultSubobject<UCActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";

	TargeActorKeyName = "TargetActor";
}

void ACBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ACBot::OnSeePawn);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACBot::OnHealthChanged);
}

void ACBot::OnSeePawn(APawn* Pawn)
{
	 AAIController* AIC = GetController<AAIController>();
	 if (ensure(AIC && SpottedWidgetClass))
	 {
		 if (GetTargetActor() != Pawn)
		 {
			 SetTargetActor(Pawn);
			 NetMulticastPawnSeen();
			
		 }
	 }
}

void ACBot::NetMulticastPawnSeen_Implementation()
{
	UCWorldWidget* SpottedWidget = CreateWidget<UCWorldWidget>(GetWorld(), SpottedWidgetClass);
	if (SpottedWidget)
	{
		SpottedWidget->AttachToActor = this;
		SpottedWidget->AddToViewport(10);
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

		if (NewHealth <= 0.f)
		{
			AAIController* AIC = GetController<AAIController>();
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			SetLifeSpan(10.f);
			return;
		}
		if (HealthBarWidget == nullptr)
		{
			HealthBarWidget= CreateWidget<UCWorldWidget>(GetWorld(), HealthBarWidgetClass);
			if (HealthBarWidget)
			{
			HealthBarWidget->AttachToActor = this;
			HealthBarWidget->AddToViewport();
			}
		}

	}
}

void ACBot::SetTargetActor(AActor* NewTarget)
{

	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
	AIC->GetBlackboardComponent()->SetValueAsObject(TargeActorKeyName, NewTarget);
	}
}


AActor* ACBot::GetTargetActor() const
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargeActorKeyName));
	}

	return nullptr;
}