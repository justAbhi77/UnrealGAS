//

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * Gameplay Mod Magnitude Calculation for Max Health.
 * Calculates the maximum health based on Vigor attribute and player level.
 */
UCLASS()
class AURA_API UMmc_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMmc_MaxHealth();

	/**
	 * Calculates the base magnitude for the Gameplay Effect.
	 * @param Spec The Gameplay Effect Spec containing context and captured attributes.
	 * @return The calculated base magnitude for max health.
	 */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// Capture definition for the Vigor attribute.
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
