#include "CAttributeComponent.h"
#include "Game/CGameMode.h"

TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("Tore.DamageMultiplier"), 1.f, TEXT("Modify damage Multiplier"), ECVF_Cheat);

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

	if (Delta < 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float PrevHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);

	float ActualDelta = Health - PrevHealth;

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(Instigatorctor,this, Health, ActualDelta);
	}

	if (ActualDelta < 0.f)
	{
		if (OnChargehChanged.IsBound())
		{
			OnChargehChanged.Broadcast(ActualDelta);
		}
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

bool UCAttributeComponent::IsFullCharge() const
{
	return (Charge >= MaxCharge);
}

float UCAttributeComponent::GetHealth() const
{
	return Health;
}

float UCAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UCAttributeComponent::GetMaxCharge() const
{
	return MaxCharge;
}

void UCAttributeComponent::PlusCharge(float Delta) 
{
	if (Charge < MaxCharge)
	{
	 Charge += Delta;
	}
}

bool UCAttributeComponent::Kill(AActor* Killer)
{
	return ApplyHealthChange(Killer,-GetMaxHealth());
}

void UCAttributeComponent::UsedCharge()
{
	Charge = 0;
}


