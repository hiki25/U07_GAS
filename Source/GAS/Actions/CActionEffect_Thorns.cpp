#include "CActionEffect_Thorns.h"
#include "Components/CActionComponent.h"
#include "Components/CAttributeComponent.h"
#include "Game/CFunctionLibrary.h"

UCActionEffect_Thorns::UCActionEffect_Thorns()
{
	Period = 0.f;
	Duration = 0.f;

	ReflectRatio = 0.2f;
}

void UCActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UCActionEffect_Thorns::OnHealthChanged);
	}
}

void UCActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(GetOwningComponent()->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UCActionEffect_Thorns::OnHealthChanged);
	}
}

void UCActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewValue, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (Delta < 0.f && OwningActor != InstigatorActor)
	{
		int32 RoundDamage = FMath::RoundToInt(FMath::Abs(Delta)* ReflectRatio);
		if (RoundDamage == 0)
		{
			return;
		}

		UCFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, RoundDamage);
	}
}