//

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;
struct FGameplayTag;

// Delegate for broadcasting attribute information to UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * Controller for the Attribute Menu Widget.
 * Handles broadcasting and binding of gameplay attribute changes to UI elements.
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	// Broadcasts the initial values of attributes to the UI.
	virtual void BroadcastInitialValues() override;

	 // Binds callbacks to the gameplay attribute dependencies.
	virtual void BindCallbacksToDependencies() override;

	// Delegate to notify UI about attribute information updates.
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	// Reference to the AttributeInfo asset, containing metadata for attributes.
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	/**
	 * Helper function to broadcast attribute information for a specific attribute tag.
	 * @param AttributeTag - The tag representing the attribute.
	 * @param Attribute - The gameplay attribute being updated.
	 */
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
