// 

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 * Extends UAuraDamageGameplayAbility to implement projectile-based spell abilities.
 * Handles projectile spawning and applies damage effects dynamically.
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	// The class of projectile that this ability spawns.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<AAuraProjectile> ProjectileClass;

	// Spawns a projectile at the weapon socket location and sets its properties.
	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
