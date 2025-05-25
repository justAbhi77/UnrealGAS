//


#include "UI/WidgetController/AuraWidgetController.h"
#include "Aura/AuraLogChannels.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	UE_LOG(LogAura, Display, TEXT("Broadcasting initial values from [%hs]"), __FUNCTION__);
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	UE_LOG(LogAura, Display, TEXT("Binding call backs to dependencies from [%hs]"), __FUNCTION__);
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraAsc()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = AuraAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraAsc()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPc()
{
	if(AuraPlayerController == nullptr)
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPs()
{
	if(AuraPlayerState == nullptr)
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAsc()
{
	if(AuraAbilitySystemComponent == nullptr)
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAs()
{
	if(AuraAttributeSet == nullptr)
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	return AuraAttributeSet;
}