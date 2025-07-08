//


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/AuraAttributeSet.h"

float UAuraGameplayAbility::GetManaCost(const float InLevel) const
{
	float ManaCost = 0.f;

	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
		for(FGameplayModifierInfo Mod : CostEffect->Modifiers)
			if(Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}

	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(const float InLevel) const
{
	float Cooldown = 0.f;

	if(const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);

	return Cooldown;
}
