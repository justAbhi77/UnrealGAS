//

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * Struct that holds XP requirement and point awards for a specific level
 */
USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	// Experience points required to reach this level
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	// Attribute points awarded upon reaching this level
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	// Spell points awarded upon reaching this level
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 * Data asset that maps XP to level progression info
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// Level data starting from index 1 (index 0 is unused)
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	// Finds the level corresponding to given XP
	int32 FindLevelForXP(int32 XP) const;
};
