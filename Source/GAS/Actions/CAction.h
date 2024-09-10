#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CActionComponent.h"
#include "GameplayTagContainer.h"
#include "CAction.generated.h"

class UCActionComponent;
class UTexture2D;

UCLASS(Blueprintable)
class GAS_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;


	void SetOwningComponent(UCActionComponent* NewActionComp);

	UWorld* GetWorld() const override; 

protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
protected:
	//Gameplay Tag
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer BlockTags;

	//Replicates
	UPROPERTY(ReplicatedUsing = "OnRep_IsRunning")
		FActionRepData RepData;

	UFUNCTION()
	void OnRep_IsRunning();

	UPROPERTY(Replicated)
	UCActionComponent* ActionComp;

	UPROPERTY(Replicated)
	float TimeStarted;

	//Icon
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	UTexture2D* Icon;
};
