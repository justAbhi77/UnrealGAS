//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;

/**
 * Multicast Delegate to keep dependencies one way
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);

/**
 * Custom Ability System Component for handling Aura-specific gameplay logic.
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Initializes the ability actor info and binds gameplay effect delegates.
	void AbilityActorInfoSet();

	// Event for tags of applied gameplay effects.
	FEffectAssetTags EffectAssetTags;

	// Event for successful application of gameplay abilities.
	FAbilitiesGiven AbilitiesGivenDelegate;

	// Event for when the ability status changes.
	FAbilityStatusChanged AbilityStatusChangedDelegate;

	// Event for when the ability is equipped.
	FAbilityEquipped AbilityEquipped;

	// Event for when a passive ability deactivates.
	FDeactivatePassiveAbility DeactivatePassiveAbility;

	// Event for when a passive ability activates.
	FActivatePassiveEffect ActivatePassiveEffect;

	/**
	 * Adds abilities to the character and binds them to their input tags.
	 * @param StartupAbilities List of gameplay abilities to add.
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	/**
	 * Adds the abilities to the character that are saved in the save data.
	 * @param SaveData A save data from a slot.
	 */
	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);

	// Flag for successful application of gameplay abilities.
	bool bStartupAbilitiesGiven = false;

	/**
	 * Adds Passive abilities to the character.
	 * @param StartupPassiveAbilities List of passive gameplay abilities to add.
	 */
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	/**
	 * Handles input pressed for abilities tied to a specific input tag.
	 * @param InputTag The tag identifying the input action.
	 */
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	/**
	 * Handles input held for abilities tied to a specific input tag.
	 * @param InputTag The tag identifying the input action.
	 */
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	/**
	 * Handles input released for abilities tied to a specific input tag.
	 * @param InputTag The tag identifying the input action.
	 */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/**
	 * Function to iterate over all activatable abilities from GetActivatableAbilities and execute a delegate for each.
	 * @param Delegate The delegate to execute for each ability.
	 */
	void ForEachAbility(const FForEachAbility& Delegate);

	/**
	 * Returns the ability tag from the given ability spec.
	 * @param AbilitySpec The ability spec to extract the tag from.
	 * @return The tag associated with the ability spec or an empty tag if not found.
	 */
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/**
	 * Returns the input tag from the given ability spec.
	 * @param AbilitySpec The ability spec to extract the input tag from.
	 * @return The input tag associated with the ability spec or an empty tag if not found.
	 */
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/**
	 * Returns the status tag from the given ability spec.
	 * @param AbilitySpec The ability spec to extract the status tag from.
	 * @return The status tag associated with the ability spec or an empty tag if not found.
	 */
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/**
	 * Returns the status from the given ability tag.
	 * @param AbilityTag The tag identifying the ability.
	 * @return Status of the ability.
	 */
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);

	/**
	 * Returns the slot from the given ability tag.
	 * @param AbilityTag The tag identifying the ability.
	 * @return The slot associated with the ability tag or an empty tag if not found.
	 */
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);

	bool SlotIsEmpty(const FGameplayTag& Slot);

	/**
	 * Checks the ability spec for a specific slot.
	 * @param Spec Reference to the ability spec to extract the slot from.
	 * @param Slot The slot tag to check.
	 * @return True if the ability spec has the specified slot, false otherwise.
	 */
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	/**
	 * Checks the ability spec for a specific slot.
	 * @param Spec Pointer to the ability spec to extract the slot from.
	 * @param Slot The slot tag to check.
	 * @return True if the ability spec has the specified slot, false otherwise.
	 */
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);

	/**
	 * Checks if the ability spec has any slot assigned.
	 * @param Spec The ability spec to check.
	 * @return True if the ability spec has any slot, false otherwise.
	 */
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);

	/**
	 * Retrieves the ability spec that has the specified slot.
	 * @param Slot The slot tag to search for.
	 * @return Pointer to the ability spec with the specified slot or nullptr if not found.
	 */
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);

	/**
	 * Checks if the ability spec is a passive ability.
	 * @param Spec The ability spec to check.
	 * @return True if the ability spec is a passive ability, false otherwise.
	 */
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;

	/**
	 * Assigns a new slot for this ability spec.
	 * @param Spec The ability spec.
	 * @param Slot The new slot for the ability.
	 */
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	// Replicate the (de)activation for the passive ability with this tag.
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);

	/**
	 * Retrieves the ability spec from the given ability tag.
	 * @param AbilityTag The tag identifying the ability.
	 * @return Pointer to the ability spec or nullptr if not found.
	 */
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	// increment the level of the Attribute by one point.
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	// Replicate the increase in the level of the Attribute by one point.
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	// change the status of an ability to eligible if player has reached a level.
	void UpdateAbilityStatuses(int32 Level);

	// Replicate the spending of a spell point for an ability.
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	// Replicate the Equipping of an ability to a slot.
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	// Replicate the Equipping of an ability to a slot.
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	/**
	 * Get a description of the ability with tag.
	 * @param AbilityTag The tag identifying the ability.
	 * @param OutDescription The description of the ability.
	 * @param OutNextLevelDescription  The description for the next level of the ability.
	 * @return true if the ability was found and descriptions were set, false otherwise.
	 */
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// Clear slot of any ability that might be equipped there.
	static void ClearSlot(FGameplayAbilitySpec* Spec);

	// Clear all abilities from this slot.
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
protected:

	virtual void OnRep_ActivateAbilities() override;

	/**
	 * Handles gameplay effects applied to the owning actor, broadcasting their asset tags.
	 * Replicated because gameplay effects can only be applied on the server
	 * @param AbilitySystemComponent The component managing abilities.
	 * @param EffectSpec The spec of the gameplay effect applied.
	 * @param ActiveEffectHandle Handle to the active effect instance.
	 */
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle);

	// Change the ability status on the client.
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
