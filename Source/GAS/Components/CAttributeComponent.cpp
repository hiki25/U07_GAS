#include "CAttributeComponent.h"
#include "Game/CGameMode.h"

UCAttributeComponent::UCAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}


void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UCAttributeComponent* UCAttributeComponent::GetAttribute(AActor* FromActor)
{
	if (FromActor)
	{
	return Cast<UCAttributeComponent>(FromActor->GetComponentByClass(UCAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UCAttributeComponent::IsActorAlive(AActor* Actor)
{
	UCAttributeComponent* AttributeComp = GetAttribute(Actor);
	if (AttributeComp)
	{
	return AttributeComp->IsAlive();

	}
	return false;
}

bool UCAttributeComponent::ApplyHealthChange(AActor* Instigatorctor,float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}

	float PrevHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);

	float ActualDelta = Health - PrevHealth;

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(Instigatorctor,this, Health, ActualDelta);
	}

	if (ActualDelta < 0.f && Health <= 0.f)
	{
		ACGameMode* GM = GetWorld()->GetAuthGameMode<ACGameMode>();
		if (ensure(GM))
		{
			GM->OnActorKilled(GetOwner(), Instigatorctor);
		}
	}

	return !FMath::IsNearlyZero(ActualDelta);
}

bool UCAttributeComponent::IsAlive() const
{
	return Health> 0.f;
}

bool UCAttributeComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Health,MaxHealth);
}

float UCAttributeComponent::GetHealth() const
{
	return Health;
}

float UCAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool UCAttributeComponent::Kill(AActor* Killer)
{
	return ApplyHealthChange(Killer,-GetMaxHealth());
}


