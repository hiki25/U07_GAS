#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "CFunctionLibrary.generated.h"

UCLASS()
class GAS_API UCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool ApplyDamage(AActor* DamageCusor, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool ApplyDirectionDamage(AActor* DamageCusor, AActor* TargetActor, float DamageAmount,const FHitResult& HitResult);
};
