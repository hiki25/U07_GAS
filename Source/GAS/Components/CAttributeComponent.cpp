#include "CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}


void UCAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UCAttributeComponent::ApplyHealthChange(float Delta)
{
	float PrevHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);

	float ActualDelath = Health - PrevHealth;

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(nullptr,this, Health, ActualDelath);
	}

	return !FMath::IsNearlyZero(ActualDelath);
}

bool UCAttributeComponent::IsAlive() const
{
	return Health> 0.f;
}

bool UCAttributeComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Health,MaxHealth);
}

float UCAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}


