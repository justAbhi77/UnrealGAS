// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

/**
 * Struct representing information about a specific attribute, including its tag, name, description, and value.
 */
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// Gameplay tag identifying the attribute (e.g., "Health", "Mana").
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	FGameplayTag AttributeTag = FGameplayTag();

	// Display name of the attribute in widgets.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	FText AttributeName = FText();

	// Detailed description of the attribute in widgets.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	FText AttributeDescription = FText();

	// Current value of the attribute.
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	float AttributeValue = 0.f;
};

/**
 * Data asset containing a list of attribute information, allowing for easy retrieval by tag.
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	* Finds and returns attribute information corresponding to the specified gameplay tag.
	*/
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	// Array storing all attribute information for this data asset.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TArray<FAuraAttributeInfo> AttributeInformation;
};
