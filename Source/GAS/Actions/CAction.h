#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CActionComponent.h"
#include "GameplayTagContainer.h"
#include "CAction.generated.h"

UCLASS(Blueprintable)
class GAS_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

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
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer BlockTags;


	bool bIsRunning;
};
