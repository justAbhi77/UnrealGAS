// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

/**
 * Interface for combat-related functionality, providing a standardized way
 * to query combat-specific data and implement combat behavior in actors.
 */
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface class that defines the combat-related functions.
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();

	/**
	 * Retrieves a location on weapon. 
	 * @return A FVector representing the socket location. Defaults to ZeroVector.
	 */
	virtual FVector GetCombatSocketLocation();
};
