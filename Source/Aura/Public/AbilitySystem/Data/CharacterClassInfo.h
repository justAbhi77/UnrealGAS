// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

// Enumeration for different character classes.
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist, // Sorcerer class specializing in elemental magic
	Warrior,      // Melee fighter with high durability
	Ranger        // Agile ranged combatant
};

// Struct containing default attributes and abilities for each character class.
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	// Gameplay effect that defines primary attributes for the class.
	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	// List of abilities that the character starts with.
	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};

/** 
* Data asset storing class-specific attributes, abilities, and common gameplay effects.
*/
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// Mapping of character classes to their respective default attributes and abilities.
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	// Gameplay effect defining secondary attributes shared by all character classes.
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	// Gameplay effect defining vital attributes like health, stamina, etc.
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	// List of abilities common to all character classes.
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	// Curve table used for calculating damage coefficients.
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	// Retrieves default info for the specified character class.
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
