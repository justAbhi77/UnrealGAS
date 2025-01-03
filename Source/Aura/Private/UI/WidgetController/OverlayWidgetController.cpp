// 


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Bind health attribute changes
	FGameplayAttribute HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	FOnGameplayAttributeValueChange& HealthAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttribute);
	HealthAttributeChanged.AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	// Bind max health attribute changes
	FGameplayAttribute MaxHealthAttribute = AuraAttributeSet->GetMaxHealthAttribute();
	FOnGameplayAttributeValueChange& MaxHealthAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxHealthAttribute);
	MaxHealthAttributeChanged.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

	// Bind mana attribute changes
	FGameplayAttribute ManaAttribute = AuraAttributeSet->GetManaAttribute();
	FOnGameplayAttributeValueChange& ManaAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManaAttribute);
	ManaAttributeChanged.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});

	// Bind max mana attribute changes
	FGameplayAttribute MaxManaAttribute = AuraAttributeSet->GetMaxManaAttribute();
	FOnGameplayAttributeValueChange& MaxManaAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxManaAttribute);
	MaxManaAttributeChanged.AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});

	// Bind gameplay tag events for message widgets
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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
				{
					if(const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
						MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
}
