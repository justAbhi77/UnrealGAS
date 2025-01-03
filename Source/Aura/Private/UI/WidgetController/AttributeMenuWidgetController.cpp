// 


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	// Ensure the AttributeSet and AttributeInfo are valid
	UAuraAttributeSet* As = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// loop through the tag-to-attribute mapping and broadcast values
	for(const auto& Pair : As->TagsToAttributes)
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// Bind a callback to value change delegate
	for(const auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});
	}
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
