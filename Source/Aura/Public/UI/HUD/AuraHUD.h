//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;
class UAuraUserWidget;
struct FWidgetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;
class USpellMenuWidgetController;

/**
 * Manages the user interface, including the overlay and attribute menu widgets,
 * and their respective widget controllers.
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves or creates the Overlay Widget Controller.
	 * @param WcParams - Parameters required to initialize the controller.
	 * @return A pointer to the Overlay Widget Controller.
	 */
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WcParams);

	/**
	 * Retrieves or creates the Attribute Menu Widget Controller.
	 * @param WcParams - Parameters required to initialize the controller.
	 * @return A pointer to the Attribute Menu Widget Controller.
	 */
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WcParams);

	/**
	 * Retrieves or creates the Spell Menu Widget Controller.
	 * @param WcParams - Parameters required to initialize the controller.
	 * @return A pointer to the Spell Menu Widget Controller.
	 */
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WcParams);

	/**
	 * Initializes the Overlay Widget, sets its controller, and adds it to the viewport.
	 * @param PC - The player controller.
	 * @param PS - The player state.
	 * @param Asc - The ability system component.
	 * @param As - The attribute set.
	 */
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* Asc, UAttributeSet* As);

private:
	// The class to use for the Overlay Widget.
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	// The instance of the Overlay Widget.
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	// The class to use for the Overlay Widget Controller.
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	// The instance of the Overlay Widget Controller.
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	// The class to use for the Attribute Menu Widget Controller.
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	// The instance of the Attribute Menu Widget Controller.
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	// The class to use for the Spell Menu Widget Controller.
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	// The instance of the Spell Menu Widget Controller.
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
};
