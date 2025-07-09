//

#pragma once

#include "CoreMinimal.h"
#include "AuraBeamSpell.h"
#include "AuraElectrocute.generated.h"

/**
 * Gameplay ability that electrocutes targets.
 */
UCLASS()
class AURA_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
