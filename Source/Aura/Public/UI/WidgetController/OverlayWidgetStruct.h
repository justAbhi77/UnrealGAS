
# pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetStruct.generated.h"

// Struct defining Widget Row data in a DataTable
USTRUCT(BlueprintType)
struct FUiWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* Image = nullptr;
};