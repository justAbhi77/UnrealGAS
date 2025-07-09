//

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/*
 * Gameplay Ability with input tag for activation, Description and cost estimating functionalities.
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Input tag used to activate this ability.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(const int32 InLevel){ return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Description coming soon.", InLevel); }
	virtual FString GetNextLevelDescription(const int32 InLevel){ return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Next Level coming soon. </>"), InLevel); }
	static FString GetLockedDescription(const int32 InLevel) { return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), InLevel); }

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};
