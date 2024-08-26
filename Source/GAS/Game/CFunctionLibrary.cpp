#include "CFunctionLibrary.h"
#include "Components/CAttributeComponent.h"

bool UCFunctionLibrary::ApplyDamage(AActor* DamageCusor, AActor* TargetActor, float DamageAmount)
{
	UCAttributeComponent* AttributeComp = UCAttributeComponent::GetAttribute(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCusor, -DamageAmount);
	}

	return false;
}

bool UCFunctionLibrary::ApplyDirectionDamage(AActor* DamageCusor, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{

	if (ApplyDamage(DamageCusor, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
