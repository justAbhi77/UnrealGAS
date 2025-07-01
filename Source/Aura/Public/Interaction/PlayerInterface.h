//


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Base interface class for player-related functionality.
 * This interface is designed to be implemented by player-related classes to handle XP, levels, attributes, and spell points.
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	// Adds XP to the player.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|XP")
	void AddToXp(int32 InXP);

	// Adds levels to the player.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Level")
	void AddToPlayerLevel(int32 InPlayerLevel);

	// Adds attribute points to the player.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Attributes")
	void AddToAttributePoints(int32 InAttributePoints);

	// Adds spell points to the player.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Spells")
	void AddToSpellPoints(int32 InSpellPoints);

	// Handles player level-up logic.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Level")
	void LevelUp();

	// Finds the appropriate level for the given XP value.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|XP")
	int32 FindLevelForXp(int32 InXp) const;

	// Retrieves the player's current XP.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|XP")
	int32 GetXp() const;

	// Calculates the attribute points reward for a given level.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Attributes")
	int32 GetAttributePointsReward(int32 Level) const;

	// Calculates the spell points reward for a given level.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Spells")
	int32 GetSpellPointsReward(int32 Level) const;

	// Retrieves the player's current attribute points.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Attributes")
	int32 GetAttributePoints() const;

	// Retrieves the player's current spell points.
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Spells")
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckpointTag);
};
