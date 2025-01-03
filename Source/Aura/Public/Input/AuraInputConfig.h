// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

/**
 * Structure representing an input action mapped to a gameplay tag.
 */
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	// The input action.
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	// The gameplay tag.
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * Data asset used to configure input actions and their mappings to gameplay tags.
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Finds the input action associated with a specific gameplay tag.
	 * 
	 * @param InputTag The gameplay tag to search for.
	 * @param bLogNotFound If true, logs an error if the action is not found.
	 * @return A pointer to the input action, or nullptr if not found.
	 */
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	// Array of input actions and their associated gameplay tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
