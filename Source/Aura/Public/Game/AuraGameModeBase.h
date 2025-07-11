//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class USaveGame;
class UMvvm_LoadSlot;
class ULoadScreenSaveGame;
class ULootTiers;

/**
 * Manages default character class information and Save state.
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	// Stores default character class information, used for enemy initialization
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditdefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void SaveSlotData(UMvvm_LoadSlot* LoadSlot, int32 SlotIndex) const;

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	void TravelToMap(const UMvvm_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	ULoadScreenSaveGame* RetrieveInGameSaveData() const;

	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const;

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;

	void LoadWorldState(UWorld* World) const;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	void PlayerDied(const ACharacter* DeadCharacter) const;

	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<ULootTiers> LootTiers;
protected:
	virtual void BeginPlay() override;
};
