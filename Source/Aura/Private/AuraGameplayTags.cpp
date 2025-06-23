//


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Aura/AuraLogChannels.h"

// Static instance definition
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UE_LOG(LogAura, Display, TEXT("Initializing native GameplayTags from [%hs]"), __FUNCTION__);

	auto& TagsManager = UGameplayTagsManager::Get();

	// Helper lambda to register tags
	auto RegisterTag = [&TagsManager](FGameplayTag& OutTag, const FName& TagName, const FString& TagDescription)
	{
		OutTag = TagsManager.AddNativeGameplayTag(TagName, TagDescription);
	};

	// Primary Attributes
	RegisterTag(GameplayTags.Attributes_Primary_Strength, "Attributes.Primary.Strength", "Increases physical damage");
	RegisterTag(GameplayTags.Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage");
	RegisterTag(GameplayTags.Attributes_Primary_Resilience, "Attributes.Primary.Resilience", "Increases armor and armor penetration");
	RegisterTag(GameplayTags.Attributes_Primary_Vigor, "Attributes.Primary.Vigor", "Increases health");

	// Secondary Attributes
	RegisterTag(GameplayTags.Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces damage taken, improves block chance");
	RegisterTag(GameplayTags.Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Ignores armor, improves critical hit chance");
	RegisterTag(GameplayTags.Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance", "Chance to reduce incoming damage");
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "Chance to deal bonus critical damage");
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Bonus damage on critical hits");
	RegisterTag(GameplayTags.Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", "Reduces incoming critical hit chance");
	RegisterTag(GameplayTags.Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Health regenerated per second");
	RegisterTag(GameplayTags.Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Mana regenerated per second");
	RegisterTag(GameplayTags.Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum obtainable health");
	RegisterTag(GameplayTags.Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", "Maximum obtainable mana");

	// Resistance Attributes
	RegisterTag(GameplayTags.Attributes_Resistance_Fire, "Attributes.Resistance.Fire", "Resistance to fire damage");
	RegisterTag(GameplayTags.Attributes_Resistance_Lighting, "Attributes.Resistance.Lighting", "Resistance to lighting damage");
	RegisterTag(GameplayTags.Attributes_Resistance_Arcane, "Attributes.Resistance.Arcane", "Resistance to arcane damage");
	RegisterTag(GameplayTags.Attributes_Resistance_Physical, "Attributes.Resistance.Physical", "Resistance to physical damage");

	// Meta Attributes
	RegisterTag(GameplayTags.Attributes_Meta_IncomingXp, "Attributes.Meta.IncomingXP", "Incoming experience points");

	// Input Tags
	RegisterTag(GameplayTags.InputTag_LMB, "InputTag.LMB", "Input for left mouse button");
	RegisterTag(GameplayTags.InputTag_RMB, "InputTag.RMB", "Input for right mouse button");
	RegisterTag(GameplayTags.InputTag_1, "InputTag.1", "Input for key 1");
	RegisterTag(GameplayTags.InputTag_2, "InputTag.2", "Input for key 2");
	RegisterTag(GameplayTags.InputTag_3, "InputTag.3", "Input for key 3");
	RegisterTag(GameplayTags.InputTag_4, "InputTag.4", "Input for key 4");

	// Passive Input Tags
	RegisterTag(GameplayTags.InputTag_Passive_1, "InputTag.Passive.1", "Input for Passive key 1");
	RegisterTag(GameplayTags.InputTag_Passive_2, "InputTag.Passive.2", "Input for Passive key 2");

	// Damage Tags
	RegisterTag(GameplayTags.Damage, "Damage", "Base damage tag");
	RegisterTag(GameplayTags.Damage_Fire, "Damage.Fire", "Fire damage type");
	RegisterTag(GameplayTags.Damage_Lighting, "Damage.Lighting", "Lighting damage type");
	RegisterTag(GameplayTags.Damage_Arcane, "Damage.Arcane", "Arcane damage type");
	RegisterTag(GameplayTags.Damage_Physical, "Damage.Physical", "Physical damage type");

	RegisterTag(GameplayTags.Abilities_None, "Abilities.None", "No Ability");

	// Ability Tags
	RegisterTag(GameplayTags.Abilities_Attack, "Abilities.Attack", "Attack ability tag");
	RegisterTag(GameplayTags.Abilities_Summon, "Abilities.Summon", "Summon ability tag");
	RegisterTag(GameplayTags.Abilities_Fire_FireBolt, "Abilities.Fire.FireBolt", "FireBolt ability tag");
	RegisterTag(GameplayTags.Abilities_Lighting_Electrocute, "Abilities.Lighting.Electrocute", "Electrocute ability tag");
	RegisterTag(GameplayTags.Abilities_Arcane_ArcaneShards , "Abilities.Arcane.ArcaneShards", "Arcane Shards Ability Tag");
	RegisterTag(GameplayTags.Abilities_Fire_FireBlast, "Abilities.Fire.FireBolt", "FireBlast Ability Tag");

	RegisterTag(GameplayTags.Abilities_HitReact, "Abilities.HitReact", "Hit reaction ability");
	RegisterTag(GameplayTags.Abilities_Status_Locked, "Abilities.Status.Locked", "Ability is locked and cannot be activated");
	RegisterTag(GameplayTags.Abilities_Status_Eligible, "Abilities.Status.Eligible", "Ability is eligible for activation");
	RegisterTag(GameplayTags.Abilities_Status_Unlocked, "Abilities.Status.Unlocked", "Ability is unlocked and can be activated");
	RegisterTag(GameplayTags.Abilities_Status_Equipped, "Abilities.Status.Equipped", "Ability is Equipped");

	
	RegisterTag(GameplayTags.Abilities_Type_Offensive, "Abilities.Type.Offensive", "Offensive ability type");
	RegisterTag(GameplayTags.Abilities_Type_Passive, "Abilities.Type.Passive", "Passive ability type");
	RegisterTag(GameplayTags.Abilities_Type_None, "Abilities.Type.None", "No ability type");

	// Cooldown Tags
	RegisterTag(GameplayTags.Cooldown_Fire_FireBolt, "Cooldown.Fire.FireBolt", "FireBolt ability cooldown");

	// Combat Socket Tags
	RegisterTag(GameplayTags.CombatSocket_Weapon, "CombatSocket.Weapon", "Weapon socket");
	RegisterTag(GameplayTags.CombatSocket_RightHand, "CombatSocket.RightHand", "Right hand socket");
	RegisterTag(GameplayTags.CombatSocket_LeftHand, "CombatSocket.LeftHand", "Left hand socket");
	RegisterTag(GameplayTags.CombatSocket_Tail, "CombatSocket.Tail", "Tail socket");

	// Montage Tags
	RegisterTag(GameplayTags.Montage_Attack_1, "Montage.Attack.1", "Attack montage 1");
	RegisterTag(GameplayTags.Montage_Attack_2, "Montage.Attack.2", "Attack montage 2");
	RegisterTag(GameplayTags.Montage_Attack_3, "Montage.Attack.3", "Attack montage 3");
	RegisterTag(GameplayTags.Montage_Attack_4, "Montage.Attack.4", "Attack montage 4");

	// Effects
	RegisterTag(GameplayTags.Effects_HitReact, "Effects.HitReact", "Tag for hit reacting");

	// Debuffs
	RegisterTag(GameplayTags.Debuff_Arcane, "Debuff.Arcane", "Debuff for Arcane damage");
	RegisterTag(GameplayTags.Debuff_Burn, "Debuff.Burn", "Debuff for Fire damage");
	RegisterTag(GameplayTags.Debuff_Physical, "Debuff.Physical", "Debuff for Physical damage");
	RegisterTag(GameplayTags.Debuff_Stun, "Debuff.Stun", "Debuff for Lighting damage");

	RegisterTag(GameplayTags.Debuff_Chance, "Debuff.Chance", "Debuff Chance");
	RegisterTag(GameplayTags.Debuff_Damage, "Debuff.Damage", "Debuff Damage");
	RegisterTag(GameplayTags.Debuff_Duration, "Debuff.Duration", "Debuff Duration");
	RegisterTag(GameplayTags.Debuff_Frequency, "Debuff.Frequency", "Debuff Frequency");

	// Player Block Tags
	RegisterTag(GameplayTags.Player_Block_CursorTrace, "Player.Block.CursorTrace", "Block tracing under the cursor");
	RegisterTag(GameplayTags.Player_Block_InputHeld, "Player.Block.InputHeld", "Block Input Held callback for input");
	RegisterTag(GameplayTags.Player_Block_InputPressed, "Player.Block.InputPressed", "Block Input Pressed callback for input");
	RegisterTag(GameplayTags.Player_Block_InputReleased, "Player.Block.InputReleased", "Block Input Released callback for input");

	RegisterTag(GameplayTags.Abilities_Passive_LifeSiphon , "Abilities.Passive.LifeSiphon", "Life Siphon");
	RegisterTag(GameplayTags.Abilities_Passive_ManaSiphon , "Abilities.Passive.ManaSiphon", "Mana Siphon");
	RegisterTag(GameplayTags.Abilities_Passive_HaloOfProtection , "Abilities.Passive.HaloOfProtection", "Halo Of Protection");

	// Map Damage Types to Resistances
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lighting, GameplayTags.Attributes_Resistance_Lighting);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	// Map Damage Types to Debuffs
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Lighting, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
}
