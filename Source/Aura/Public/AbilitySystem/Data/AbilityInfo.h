//

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

/**
 * Holds metadata for an ability, such as tags and UI assets
 */
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	// Primary ability tag to identify the ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	// Tag used for identifying cooldown effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	// Which ability type this is, e.g. "Offensive" or "Passive
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	// Icon used for UI display
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	// Background material used for UI slot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	// At what level is this ability available to unlock
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	// The gameplay ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	// Input tag associated with the ability
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	// Whether the ability is currently Equipped
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();
};

/**
 * Data asset that maps ability tags to their metadata for UI and logic use
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// List of all ability info entries
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;

	// Finds ability info by tag; optionally logs error if not found
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};