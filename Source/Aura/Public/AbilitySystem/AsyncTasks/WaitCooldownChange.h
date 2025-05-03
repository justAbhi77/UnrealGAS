//


#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 *  Async task for when a Gameplay Tag is newly added or Completely removed from the Ability System Component
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// Broadcast when the cooldown starts with time remaining
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	// Broadcast when the cooldown ends
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	// Initializes and binds delegates for cooldown tracking
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	// Stops listening and marks object for destruction
	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:
	// Owning ability system component
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	// Tag to track cooldown status
	FGameplayTag CooldownTag;

	// Called when the cooldown tag is newly added or completely removed
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	// Called when a new gameplay effect is added to the ASC
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};