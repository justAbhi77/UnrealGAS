// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

/**
 * Interface for enemy-related functionality, providing standardized methods for
 * visual effects or behaviors that can be applied to enemy actors.
 */
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface class that defines the methods for enemy-related behavior.
 * Classes implementing this interface are expected to define the provided functions.
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	/**
	 * Highlights the actor, for visual feedback while hovering.
	 */
	virtual void HighlightActor() PURE_VIRTUAL(IEnemyInterface::HighlightActor);

	/**
	 * Removes any highlight effect applied to the actor.
	 */
	virtual void UnHighlightActor() PURE_VIRTUAL(IEnemyInterface::UnHighlightActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
