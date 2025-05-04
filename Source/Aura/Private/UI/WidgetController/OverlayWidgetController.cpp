//


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayTagContainer.h"
#include "Templates/Casts.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	OnHealthChanged.Broadcast(GetAuraAs()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAs()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAs()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAs()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetAuraPs()->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXpChanged);
	GetAuraPs()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

	// Health changes
	FGameplayAttribute HealthAttribute = GetAuraAs()->GetHealthAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Max health changes
	FGameplayAttribute MaxHealthAttribute = GetAuraAs()->GetMaxHealthAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxHealthAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	// Mana changes
	FGameplayAttribute ManaAttribute = GetAuraAs()->GetManaAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManaAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	// Max mana changes
	FGameplayAttribute MaxManaAttribute = GetAuraAs()->GetMaxManaAttribute();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxManaAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	// Gameplay effect message handling
	if(GetAuraAsc())
	{
		if(GetAuraAsc()->bStartupAbilitiesGiven)
			BroadcastAbilityInfo();
		else
			GetAuraAsc()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);

		GetAuraAsc()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for(const FGameplayTag& Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True,
					// "Message".MatchesTag("Message.HealthPotion") will return False
					// root tag to check for messages to be spawned in ui
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName(Message));
					if(Tag.MatchesTag(MessageTag))
						if(const FUiWidgetRow* Row = GetDataTableRowByTag<FUiWidgetRow>(MessageWidgetDataTable, Tag))
							MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		);
	}
}

void UOverlayWidgetController::OnXpChanged(int32 NewXP)
{
	const AAuraPlayerState* ValidAuraPs = GetAuraPs();
	const ULevelUpInfo* LevelUpInfo = ValidAuraPs->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if(Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
