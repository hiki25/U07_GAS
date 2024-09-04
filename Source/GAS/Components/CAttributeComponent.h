#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine.h"
#include "CAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, class UCAttributeComponent*, OwningComp,float, NewVaule ,float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static UCAttributeComponent* GetAttribute(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);


	//체력에 대한 변화량
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyHealthChange(AActor* Instigatorctor,float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastHealthChanged(AActor* Instigatorctor, float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Cheat")
	bool Kill(AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool ApplyRageChange(AActor* Instigatorctor, float Delta);

	//UFUNCTION(NetMulticast, Reliable)
	//void NetMulticastRageChanged(AActor* Instigatorctor, float NewHealth, float Delta);


public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attribute")
	float Health;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attribute")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attribute")
	float Rage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated, Category = "Attribute")
	float MaxRage;
		
};
