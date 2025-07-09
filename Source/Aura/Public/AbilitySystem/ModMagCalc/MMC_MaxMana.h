//

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * Gameplay Mod Magnitude Calculation for Max Mana.
 * Calculates the maximum Mana based on Intelligence attribute and player level.
 */
UCLASS()
class AURA_API UMmc_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMmc_MaxMana();

	// Calculates the base magnitude for the Gameplay Effect.
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// Capture definition for the Intelligence attribute.
	FGameplayEffectAttributeCaptureDefinition IntDef;
};
