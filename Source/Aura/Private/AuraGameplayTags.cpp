// 


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

// Singleton instance definition
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

/**
 * Initializes and registers all native gameplay tags.
 * Tags are added with descriptive names and tooltips to be use in widgets.
 */
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	auto& TagsManager = UGameplayTagsManager::Get();

	// Helper lambda to register tags
	auto RegisterTag = [&TagsManager](FGameplayTag& OutTag, const FName& TagName, const FString& TagDescription)
	{
		OutTag = TagsManager.AddNativeGameplayTag(TagName, TagDescription);
	};

	// Primary Attributes
	RegisterTag(GameplayTags.Attributes_Primary_Strength,
		TEXT("Attributes.Primary.Strength"), TEXT("Increases physical damage"));
	RegisterTag(GameplayTags.Attributes_Primary_Intelligence,
		TEXT("Attributes.Primary.Intelligence"), TEXT("Increases magical damage"));
	RegisterTag(GameplayTags.Attributes_Primary_Resilience,
		TEXT("Attributes.Primary.Resilience"), TEXT("Increases Armor and Armor Penetration"));
	RegisterTag(GameplayTags.Attributes_Primary_Vigor,
		TEXT("Attributes.Primary.Vigor"), TEXT("Increases Health"));

	// Secondary Attributes
	RegisterTag(GameplayTags.Attributes_Secondary_Armor,
		TEXT("Attributes.Secondary.Armor"), TEXT("Reduces damage taken, improves Block Chance"));
	RegisterTag(GameplayTags.Attributes_Secondary_ArmorPenetration,
		TEXT("Attributes.Secondary.ArmorPenetration"), TEXT("Ignores percentage of enemy Armor, increases Critical Hit Chance"));
	RegisterTag(GameplayTags.Attributes_Secondary_BlockChance,
		TEXT("Attributes.Secondary.BlockChance"), TEXT("Chance to cut incoming damage in half"));
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitChance,
		TEXT("Attributes.Secondary.CriticalHitChance"), TEXT("Chance to double damage plus critical hit bonus"));
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitDamage,
		TEXT("Attributes.Secondary.CriticalHitDamage"), TEXT("Bonus damage added when a critical hit is scored"));
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitResistance,
		TEXT("Attributes.Secondary.CriticalHitResistance"),
		TEXT("Reduces Critical Hit Chance of attacking enemies"));
	RegisterTag(GameplayTags.Attributes_Secondary_HealthRegeneration,
		TEXT("Attributes.Secondary.HealthRegeneration"), TEXT("Amount of Health regenerated every 1 second"));
	RegisterTag(GameplayTags.Attributes_Secondary_ManaRegeneration,
		TEXT("Attributes.Secondary.ManaRegeneration"), TEXT("Amount of Mana regenerated every 1 second"));
	RegisterTag(GameplayTags.Attributes_Secondary_MaxHealth,
		TEXT("Attributes.Secondary.MaxHealth"), TEXT("Maximum amount of Health obtainable"));
	RegisterTag(GameplayTags.Attributes_Secondary_MaxMana,
		TEXT("Attributes.Secondary.MaxMana"), TEXT("Maximum amount of Mana obtainable"));

	// Input Tags
	RegisterTag(GameplayTags.InputTag_LMB, TEXT("InputTag.LMB"), TEXT("Input Tag for Left Mouse Button"));
	RegisterTag(GameplayTags.InputTag_RMB, TEXT("InputTag.RMB"), TEXT("Input Tag for Right Mouse Button"));
	RegisterTag(GameplayTags.InputTag_1, TEXT("InputTag.1"), TEXT("Input Tag for 1 key"));
	RegisterTag(GameplayTags.InputTag_2, TEXT("InputTag.2"), TEXT("Input Tag for 2 key"));
	RegisterTag(GameplayTags.InputTag_3, TEXT("InputTag.3"), TEXT("Input Tag for 3 key"));
	RegisterTag(GameplayTags.InputTag_4, TEXT("InputTag.4"), TEXT("Input Tag for 4 key"));

	RegisterTag(GameplayTags.Damage, TEXT("Damage"), TEXT("Damage"));
	RegisterTag(GameplayTags.Damage_Fire, TEXT("Damage.Fire"), TEXT("Fire Damage Type"));

	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);

	RegisterTag(GameplayTags.Effects_HitReact, TEXT("Effects.HitReact"), TEXT("Tag granted when Hit Reacting"));  
}
