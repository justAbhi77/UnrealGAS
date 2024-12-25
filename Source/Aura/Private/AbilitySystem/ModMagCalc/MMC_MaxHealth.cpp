// 


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	// since we are applying effect from aura character to aura character so both source and target are same
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// snapshotting is the time to capture the attribute(on creation or on application)
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	// check for negative value
	Vigor = FMath::Max<float>(Vigor, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	if(!CombatInterface) return 0.f; // check for combat interface

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
