// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * Extends UAuraGameplayAbility and specializes in causing damage.
 * It applies a gameplay effect to a target and allows for different damage types.
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	/** 
	 * Causes damage to the specified TargetActor using a predefined DamageEffectClass.
	 * The damage values are determined based on the associated DamageTypes map.
	 * @param TargetActor The actor that will receive the damage effect.
	 */
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	// Gameplay effect class that defines the damage applied by this ability.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** 
	 * A map containing various damage types and their corresponding scalable values.
	 * The values are adjusted based on the ability's level.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	/** 
	 * Selects a random animation montage from the provided array.
	 * @param TaggedMontages The array of tagged montages to select from.
	 * @return A randomly selected FTaggedMontage.
	 */
	UFUNCTION(BlueprintPure)
	static FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);

	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
};
