//


#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "OverlayWidgetStruct.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;
class UAbilityInfo;
class UAuraAbilitySystemComponent;
struct FAuraAbilityInfo;

// Delegate for broadcasting changes in attribute values
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

// Delegate for broadcasting message widget data
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUiWidgetRow, Row);

/**
 * Controller for managing the overlay widget, such as health/mana updates and messages.
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	// Broadcasts initial values to the UI widgets.
	virtual void BroadcastInitialValues() override;

	// Binds callbacks to external dependencies.
	virtual void BindCallbacksToDependencies() override;

	// Delegates for attribute change notifications
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	// Delegates for attribute change notifications
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// Delegates for attribute change notifications
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	// Delegates for attribute change notifications
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	// Root tag to identify messages in the gameplay tag hierarchy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Messages")
	FString Message = "Message";

	// Delegate for broadcasting UI message widget rows
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	// Delegate for broadcasting changes in Experience points
	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	// Delegate for broadcasting changes in Player level
	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

protected:
	// DataTable containing message widget configurations
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/**
	 * Retrieves a row from a DataTable using a GameplayTag as a key.
	 * @param DataTable - The DataTable to search.
	 * @param Tag - The GameplayTag to use as a key.
	 * @return A pointer to the row data.
	 */
	template<typename T>
	static T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	// Broadcast initial values for all startup abilities given to this Ability System Comp
	// void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent) const;

	// Calculate level when experience points changes
	void OnXpChanged(int32 NewXp);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
