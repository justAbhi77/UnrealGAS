//

#include "AbilitySystem/Abilities/Damage/Projectile/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch, const float PitchOverride, AActor* HomingTarget) const
{
	// check for valid data
	if(!GetAvatarActorFromActorInfo() || !ProjectileClass) return;

	// Ensure running only on the server
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	// Get socket location from the combat interface
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);

	// Calculate projectile direction
	FRotator Rotation = (ProjectileTargetLocation-SocketLocation).Rotation();
	if(bOverridePitch) Rotation.Pitch = PitchOverride;
	const FTransform SpawnTransform(Rotation, SocketLocation);

	// Spawn the projectile using deferred spawning for better setup control
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
		GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if(!Projectile) return;

	/**
	// Create effect context for applying damage effects
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	EffectContextHandle.AddActors({Projectile});
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);

	// Generate damage effect spec
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	// Apply damage types with scaled values
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);

	Projectile->DamageEffectSpecHandle = SpecHandle;
	*/

	// Set up homing target
	if(HomingTarget && HomingTarget->Implements<UCombatInterface>())
		Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
	else
	{
		Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
		Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
	}
	Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
	Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	// Complete the spawning process
	Projectile->FinishSpawning(SpawnTransform);
}
