//

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * Widget controller for managing the spell menu, including ability info.
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	// Broadcasts initial values to the UI widgets.
	virtual void BroadcastInitialValues() override;

	// Binds callbacks to external dependencies.
	virtual void BindCallbacksToDependencies() override;
};
