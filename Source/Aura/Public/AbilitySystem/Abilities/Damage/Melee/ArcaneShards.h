//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Damage/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * Gameplay ability for summoning multiple arcane shards that deal radial damage.
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(const int32 InLevel) override;
	virtual FString GetNextLevelDescription(const int32 InLevel) override;

	// Maximum number of shards that can be summoned.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;
};
