//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * UAuraUserWidget serves as a base class for custom user widgets in the Aura project.
 * It provides functionality to assign a widget controller and notifies Widgets when it's set.
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Sets the widget controller for this widget.
	 * @param InWidgetController - The widget controller to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Widget")
	void SetWidgetController(UObject* InWidgetController);

	// Holds a reference to the assigned widget controller.
	UPROPERTY(BlueprintReadOnly, Category = "UI|Widget")
	TObjectPtr<UObject> WidgetController;

protected:
	/**
	 * Widget Blueprint event triggered after the controller is set.
	 * Allows Widgets to perform additional setup when the controller is assigned.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Widget")
	void WidgetControllerSet();
};
