#include "CAttributeComponent.h"
#include "Game/CGameMode.h"
#include "Net/UnrealNetwork.h"

TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("Tore.DamageMultiplier"), 1.f, TEXT("Modify damage Multiplier"), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
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

	if (!FMath::IsNearlyZero(ActualDelta))
	{
	NetMulticastHealthChanged(Instigatorctor, Health,ActualDelta);
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

void UCAttributeComponent::NetMulticastHealthChanged_Implementation(AActor* Instigatorctor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(Instigatorctor, this, NewHealth, Delta);	
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


void UCAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAttributeComponent, Health);
	DOREPLIFETIME(UCAttributeComponent, MaxHealth);
}

