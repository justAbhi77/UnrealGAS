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
		"Attributes.Primary.Strength", "Increases physical damage");
	RegisterTag(GameplayTags.Attributes_Primary_Intelligence,
		"Attributes.Primary.Intelligence", "Increases magical damage");
	RegisterTag(GameplayTags.Attributes_Primary_Resilience,
		"Attributes.Primary.Resilience", "Increases Armor and Armor Penetration");
	RegisterTag(GameplayTags.Attributes_Primary_Vigor,
		"Attributes.Primary.Vigor", "Increases Health");

	// Secondary Attributes
	RegisterTag(GameplayTags.Attributes_Secondary_Armor,
		"Attributes.Secondary.Armor", "Reduces damage taken, improves Block Chance");
	RegisterTag(GameplayTags.Attributes_Secondary_ArmorPenetration,
		"Attributes.Secondary.ArmorPenetration", "Ignores percentage of enemy Armor, increases Critical Hit Chance");
	RegisterTag(GameplayTags.Attributes_Secondary_BlockChance,
		"Attributes.Secondary.BlockChance", "Chance to cut incoming damage in half");
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitChance,
		"Attributes.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus");
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitDamage,
		"Attributes.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored");
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitResistance,
		"Attributes.Secondary.CriticalHitResistance",
		"Reduces Critical Hit Chance of attacking enemies");
	RegisterTag(GameplayTags.Attributes_Secondary_HealthRegeneration,
		"Attributes.Secondary.HealthRegeneration", "Amount of Health regenerated every 1 second");
	RegisterTag(GameplayTags.Attributes_Secondary_ManaRegeneration,
		"Attributes.Secondary.ManaRegeneration", "Amount of Mana regenerated every 1 second");
	RegisterTag(GameplayTags.Attributes_Secondary_MaxHealth,
		"Attributes.Secondary.MaxHealth", "Maximum amount of Health obtainable");
	RegisterTag(GameplayTags.Attributes_Secondary_MaxMana,
		"Attributes.Secondary.MaxMana", "Maximum amount of Mana obtainable");

	// Input Tags
	RegisterTag(GameplayTags.InputTag_LMB, "InputTag.LMB", "Input Tag for Left Mouse Button");
	RegisterTag(GameplayTags.InputTag_RMB, "InputTag.RMB", "Input Tag for Right Mouse Button");
	RegisterTag(GameplayTags.InputTag_1, "InputTag.1", "Input Tag for 1 key");
	RegisterTag(GameplayTags.InputTag_2, "InputTag.2", "Input Tag for 2 key");
	RegisterTag(GameplayTags.InputTag_3, "InputTag.3", "Input Tag for 3 key");
	RegisterTag(GameplayTags.InputTag_4, "InputTag.4", "Input Tag for 4 key");

	RegisterTag(GameplayTags.Damage, "Damage", "Damage");
	RegisterTag(GameplayTags.Damage_Fire, "Damage.Fire", "Fire Damage Type");
	RegisterTag(GameplayTags.Damage_Lighting, "Damage.Lighting", "Lighting Damage Type");
	RegisterTag(GameplayTags.Damage_Arcane, "Damage.Arcane", "Arcane Damage Type");
	RegisterTag(GameplayTags.Damage_Physical, "Damage.Physical", "Physical Damage Type");

	RegisterTag(GameplayTags.Attributes_Resistance_Arcane, "Attributes.Resistance.Arcane", "Resistance to Arcane Damage");
	RegisterTag(GameplayTags.Attributes_Resistance_Fire, "Attributes.Resistance.Fire", "Resistance to Fire Damage");
	RegisterTag(GameplayTags.Attributes_Resistance_Lighting, "Attributes.Resistance.Lighting", "Resistance to Lighting Damage");
	RegisterTag(GameplayTags.Attributes_Resistance_Physical, "Attributes.Resistance.Physical", "Resistance to Physical Damage");

	RegisterTag(GameplayTags.Abilities_Attack, "Abilities.Attack", "Attack Ability Tag");
	RegisterTag(GameplayTags.Abilities_Summon, "Abilities.Summon", "Summon Ability Tag");
	
	RegisterTag(GameplayTags.Abilities_Fire_FireBolt, "Abilities.Fire.FireBolt", "FireBolt Ability Tag");

	RegisterTag(GameplayTags.Cooldown_Fire_FireBolt, "Cooldown.Fire.FireBolt", "FireBolt Cooldown Tag");

	RegisterTag(GameplayTags.CombatSocket_Weapon, "CombatSocket.Weapon", "Weapon");
	RegisterTag(GameplayTags.CombatSocket_RightHand, "CombatSocket.RightHand", "Right Hand");
	RegisterTag(GameplayTags.CombatSocket_LeftHand, "CombatSocket.LeftHand", "Left Hand");
	RegisterTag(GameplayTags.CombatSocket_Tail, "CombatSocket.Tail", "Tail");

	RegisterTag(GameplayTags.Montage_Attack_1, "Montage.Attack.1","Attack 1");
	RegisterTag(GameplayTags.Montage_Attack_2, "Montage.Attack.2","Attack 2");
	RegisterTag(GameplayTags.Montage_Attack_3, "Montage.Attack.3","Attack 3");
	RegisterTag(GameplayTags.Montage_Attack_4, "Montage.Attack.4","Attack 4");

	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lighting, GameplayTags.Attributes_Resistance_Lighting);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);

	RegisterTag(GameplayTags.Effects_HitReact, "Effects.HitReact", "Tag granted when Hit Reacting");

	RegisterTag(GameplayTags.Attributes_Meta_IncomingXp, "Attributes.Meta.IncomingXP", "Incoming XP Meta Attribute");
}
