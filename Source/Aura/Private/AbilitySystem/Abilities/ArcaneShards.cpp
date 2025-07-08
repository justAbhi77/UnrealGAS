//


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(const int32 InLevel)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(InLevel);
	const float ManaCost = FMath::Abs(GetManaCost(InLevel));
	const float Cooldown = GetCooldown(InLevel);

	// Level 1: single shard, otherwise: multiple shards
	if(InLevel == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Summon a shard of arcane energy, "
			"causing radial arcane damage of  </>"

			// Damage
			"<Damage>%d</><Default> at the shard origin.</>"),

			// Values
			InLevel,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of shards
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"

			// Damage
			"<Damage>%d</><Default> at the shard origins.</>"),

			// Values
			InLevel,
			ManaCost,
			Cooldown,
			FMath::Min(InLevel, MaxNumShards),
			ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 InLevel)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(InLevel);
	const float ManaCost = FMath::Abs(GetManaCost(InLevel));
	const float Cooldown = GetCooldown(InLevel);

	return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL: </>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of shards
			"<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"

			// Damage
			"<Damage>%d</><Default> at the shard origins.</>"),

			// Values
			InLevel,
			ManaCost,
			Cooldown,
			FMath::Min(InLevel, MaxNumShards),
			ScaledDamage);
}
