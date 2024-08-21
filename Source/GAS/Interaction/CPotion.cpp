#include "CPotion.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CAttributeComponent.h"

ACPotion::ACPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UCAttributeComponent* AttributeComp = Cast<UCAttributeComponent>(InstigatorPawn->GetComponentByClass(UCAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetMaxHealth()))
		{
			HideAndCooldown();
		}
	}
}
