//

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * Niagara component that handles activation based on Debuff Gameplay Tags.
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();

	// The tag associated with this debuff component
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

protected:
	// bind the appropriate function to the owner's Ability System Component.
	virtual void BeginPlay() override;

	// listen to the owner's Ability system component's tag changes.
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
