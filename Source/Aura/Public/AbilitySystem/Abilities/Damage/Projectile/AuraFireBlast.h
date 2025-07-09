//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Damage/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;

/**
 * Gameplay ability for launching fireballs in all directions, each returning and exploding.
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 InLevel) override;
	virtual FString GetNextLevelDescription(int32 InLevel) override;

	// Spawns fireballs in a radial pattern and returns them.
	UFUNCTION(BlueprintCallable, Category = "FireBlast")
	TArray<AAuraFireBall*> SpawnFireBalls() const;

protected:
	// Number of fireballs to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

private:
	// Fireball actor class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	TSubclassOf<AAuraFireBall> FireBallClass;
};
