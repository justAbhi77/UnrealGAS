// 


#include "UI/WidgetController/OverlayWidgetController.h"
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

	FGameplayAttribute HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	FOnGameplayAttributeValueChange& HealthAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttribute);
	HealthAttributeChanged.AddUObject(this, &UOverlayWidgetController::HealthChanged);
	
	FGameplayAttribute MaxHealthAttribute = AuraAttributeSet->GetMaxHealthAttribute();
	FOnGameplayAttributeValueChange& MaxHealthAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxHealthAttribute);
	MaxHealthAttributeChanged.AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	
	FGameplayAttribute ManaAttribute = AuraAttributeSet->GetManaAttribute();
	FOnGameplayAttributeValueChange& ManaAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ManaAttribute);
	ManaAttributeChanged.AddUObject(this, &UOverlayWidgetController::ManaChanged);
	
	FGameplayAttribute MaxManaAttribute = AuraAttributeSet->GetMaxManaAttribute();
	FOnGameplayAttributeValueChange& MaxManaAttributeChanged = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxManaAttribute);
	MaxManaAttributeChanged.AddUObject(this, &UOverlayWidgetController::MaxManaChanged);	
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
