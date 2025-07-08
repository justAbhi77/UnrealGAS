//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * Gameplay ability for launching fire bolts that can home in on targets and explode on impact.
 * Inherits from UAuraProjectileSpell.
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 InLevel) override;
	virtual FString GetNextLevelDescription(int32 InLevel) override;

protected:
	// Spawns fire bolt projectiles toward a target location, with optional homing and pitch override.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "FireBolt")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget) const;

	// Angle (in degrees) over which projectiles are spread.
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

	// Maximum number of projectiles that can be launched.
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles = 5;

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
