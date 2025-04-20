// 


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// Define the Vigor attribute capture
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	// since we are applying effect from aura character to aura character so both source and target are same
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // Capture attributes from the target
	// snapshotting is the time to capture the attribute(on creation or on application)
	VigorDef.bSnapshot = false;

	// Add Vigor attribute to the list of relevant captures
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Retrieve aggregated tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Set up parameters for attribute evaluation
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Capture the Vigor attribute
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);

	// Ensure Vigor is not negative
	Vigor = FMath::Max<float>(Vigor, 0.f);

	int32 PlayerLevel = 1;
	if(Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());

	// Calculate the max health using any formula
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
