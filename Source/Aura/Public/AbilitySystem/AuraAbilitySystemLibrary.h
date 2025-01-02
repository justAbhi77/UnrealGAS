// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;

/**
 * Blueprint Function Library for retrieving widget controllers in the Aura system.
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves the Overlay Widget Controller from the player's HUD.
	 * @param WorldContextObject The context object for accessing the world.
	 * @return A pointer to the Overlay Widget Controller, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	/**
	 * Retrieves the Attribute Menu Widget Controller from the player's HUD.
	 * @param WorldContextObject The context object for accessing the world.
	 * @return A pointer to the Attribute Menu Widget Controller, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
