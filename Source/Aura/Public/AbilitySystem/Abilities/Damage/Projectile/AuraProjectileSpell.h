//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Damage/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 * Base class projectile-based spell abilities.
 * Handles projectile(homing and non-homing) spawning and applies damage effects dynamically.
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
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Projectile")
	virtual void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr) const;

	// The Maximum number of projectiles this ability can spawn.
	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;

	// Whether Projectiles are homing.
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;

	// Minimum homing acceleration for projectiles.
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600.f;

	// Maximum homing acceleration for projectiles.
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;
};
