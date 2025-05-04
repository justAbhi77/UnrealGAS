//


#include "UI/WidgetController/SpellMenuWidgetController.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}
