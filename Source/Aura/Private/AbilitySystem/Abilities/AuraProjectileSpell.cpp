// 

#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// check for valid data
	if(!GetAvatarActorFromActorInfo() || !ProjectileClass) return;

	// Ensure running only on the server
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	// Get socket location from the combat interface
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		SocketTag);

	// Calculate projectile direction
	FRotator Rotation = (ProjectileTargetLocation-SocketLocation).Rotation();
	if(bOverridePitch)
		Rotation.Pitch = PitchOverride;
	FTransform SpawnTransform(Rotation, SocketLocation);

	// Spawn the projectile using deferred spawning for better setup control
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
		GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if(!Projectile) return;

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
	for(auto& Pair : DamageTypes)
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, Pair.Value.GetValueAtLevel(GetAbilityLevel()));
	
	Projectile->DamageEffectSpecHandle = SpecHandle;

	// Complete the spawning process
	Projectile->FinishSpawning(SpawnTransform);
}
