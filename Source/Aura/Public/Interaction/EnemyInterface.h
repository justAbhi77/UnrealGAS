//


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for enemy-related functionality, providing standardized methods for
 * visual effects or behaviors that can be applied to enemy actors.
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	// Highlights the actor, for visual feedback while hovering.
	virtual void HighlightActor() PURE_VIRTUAL(IEnemyInterface::HighlightActor);

	// Removes any highlight effect applied to the actor.
	virtual void UnHighlightActor() PURE_VIRTUAL(IEnemyInterface::UnHighlightActor);

	// Set the current target
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	// Get the current target
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
