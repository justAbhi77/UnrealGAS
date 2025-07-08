//

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * Extends UAuraGameplayAbility to apply damage effects to targets, supporting various damage types and parameters.
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * Applies a damage effect to the specified TargetActor using the DamageEffectClass.
	 * @param TargetActor The actor that will receive the damage effect.
	 */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void CauseDamage(AActor* TargetActor) const;

	/**
	 * Constructs FDamageEffectParams using class defaults and optional overrides.
	 * @param TargetActor Target to receive damage.
	 * @param InRadialDamageOrigin Origin for radial damage.
	 * @param bOverrideKnockbackDirection Whether to override knockback direction.
	 * @param KnockbackDirectionOverride Custom knockback direction.
	 * @param bOverrideDeathImpulse Whether to override death impulse direction.
	 * @param DeathImpulseDirectionOverride Custom death impulse direction.
	 * @param bOverridePitch Whether to override pitch.
	 * @param PitchOverride Custom pitch value.
	 */
	UFUNCTION(BlueprintPure, Category = "Damage")
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector, bool bOverrideKnockbackDirection = false, FVector KnockbackDirectionOverride = FVector::ZeroVector, bool bOverrideDeathImpulse = false, FVector DeathImpulseDirectionOverride = FVector::ZeroVector, bool bOverridePitch = false, float PitchOverride = 0.f) const;

	// Returns the damage value at the current ability level.
	UFUNCTION(BlueprintPure, Category = "Damage")
	float GetDamageThisLevel() const { return Damage.GetValueAtLevel(GetAbilityLevel()); }

	/**
	 * Selects a random animation montage from the provided array.
	 * @param TaggedMontages The array of tagged montages to select from.
	 * @return A randomly selected FTaggedMontage.
	 */
	UFUNCTION(BlueprintPure, Category = "Montage")
	static FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);

protected:
	// Gameplay effect class that defines the damage applied by this ability.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Tag representing the type of damage.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	// Scalable base damage value.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	// Percentage chance to apply a debuff.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;

	// Damage dealt by the debuff.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	// Frequency of debuff ticks (seconds).
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	// Duration of the debuff (seconds).
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;

	// Magnitude of the death impulse force.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;

	// Percentage chance to apply knockback.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;

	// Magnitude of the knockback force.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	// Whether this ability deals radial (area) damage.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	// Inner radius for radial damage.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	// Outer radius for radial damage.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;
};
