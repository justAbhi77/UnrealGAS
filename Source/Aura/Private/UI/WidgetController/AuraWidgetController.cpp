//


#include "UI/WidgetController/AuraWidgetController.h"
#include "Aura/AuraLogChannels.h"

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
