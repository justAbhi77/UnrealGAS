//


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for(const auto& AbilityClass : StartupAbilities)
	{
		// Create a new ability spec
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// we want to give an ability and activate it immediately
		// GiveAbility(AbilitySpec);
		// GiveAbilityAndActivateOnce(AbilitySpec);

		// If the ability is an AuraGameplayAbility, add its startup input tag
		if(const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec); // Grant the ability
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for(const auto& AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// Return early if the input tag is invalid
	if(!InputTag.IsValid())
		return;

	// Loop through all activatable abilities
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		// Check if this ability matches the input tag
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec); // Mark the ability as pressed
			// Activate the ability if not already active
			if(!AbilitySpec.IsActive())
				TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())
		return;

	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			AbilitySpecInputReleased(AbilitySpec); // Mark the ability as released
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		if(!Delegate.ExecuteIfBound(AbilitySpec))
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in [%hs]"), __FUNCTION__);
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if(AbilitySpec.Ability)
		for(FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
				return Tag;
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for(FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
			return Tag;
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if(GetAvatarActor()->Implements<UPlayerInterface>())
		if(IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
			ServerUpgradeAttribute(AttributeTag);
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if(GetAvatarActor()->Implements<UPlayerInterface>())
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if(!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// Collect all asset tags from the gameplay effect spec
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	// Broadcast the tags
	EffectAssetTags.Broadcast(TagContainer);
}
