//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * Multicast Delegate to keep dependencies one way
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*);

DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

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

	/**
	 * Adds abilities to the character and binds them to their input tags.
	 * @param StartupAbilities List of gameplay abilities to add.
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Flag for successful application of gameplay abilities.
	bool bStartupAbilitiesGiven = false;

	/**
	 * Adds Passive abilities to the character.
	 * @param StartupPassiveAbilities List of passive gameplay abilities to add.
	 */
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

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

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

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
};
