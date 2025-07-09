//

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMvvm_LoadSlot;

/**
 * ViewModel for the load screen in the MVVM architecture.
 */
UCLASS()
class AURA_API UMvvm_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void InitializeLoadSlots();

	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMvvm_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMvvm_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed() const;

	void LoadData();

	void SetNumLoadSlots(int32 InNumLoadSlots);
	int32 GetNumLoadSlots() const { return NumLoadSlots; }
private:
	UPROPERTY()
	TMap<int32, UMvvm_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMvvm_LoadSlot> LoadSlot_2;

	UPROPERTY()
	UMvvm_LoadSlot* SelectedSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	int32 NumLoadSlots;
};
