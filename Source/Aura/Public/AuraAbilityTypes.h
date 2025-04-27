//


#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

/**
 * Custom gameplay effect context used to carry additional information like critical hits and blocked hits.
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	// Returns the actual struct used for serialization, subclasses must override this!
	virtual UScriptStruct* GetScriptStruct() const override;

	// Creates a copy of this context, used to duplicate for later modifications (deep copy)
	virtual FAuraGameplayEffectContext* Duplicate() const override;

	// Custom network serialization, subclasses must override this
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

// Enables NetSerialize for FAuraGameplayEffectContext
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum { WithNetSerializer = true, WithCopy = true };
};
