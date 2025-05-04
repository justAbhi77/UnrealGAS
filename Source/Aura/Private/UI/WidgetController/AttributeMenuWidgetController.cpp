//


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	// Ensure the AttributeSet and AttributeInfo are valid
	UAuraAttributeSet* As = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// loop through the tag-to-attribute mapping and broadcast values
	for(const auto& Pair : As->TagsToAttributes)
		BroadcastAttributeInfo(Pair.Key, Pair.Value());

	AttributePointsChangedDelegate.Broadcast(GetAuraPs()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	// Bind a callback to value change delegate
	for(const auto& Pair : GetAuraAs()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}

	GetAuraPs()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	// Retrieve attribute metadata based on the tag
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);

	// Update the value of the attribute in the info struct
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);

	// Broadcast the updated attribute information
	AttributeInfoDelegate.Broadcast(Info);
}
