//


#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * Widget for displaying the amount of damage received
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	/**
	 * Function to set the text on the widget and change its color based on parameters.
	 * @param Damage The unit of damage to show on the screen.
	 * @param bBlockedHit Was this hit blocked?(Color changes)
	 * @param bCriticalHit Was this hit a Critical one?(Color Changes)
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);
};
