//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Damage/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * Gameplay ability base class for all beam spells that can hit a primary target and additional targets from the primary target.
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	// Store mouse hit location and actor, or cancel ability if no valid hit
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreMouseDataInfo(const FHitResult& HitResult);

	// Cache owner player controller and character for later use.
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreOwnerVariables();

	// Trace from weapon tip to target location, set first target, and bind death delegate.
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	// Find additional valid targets for beam chain and bind their death delegates
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	// Called from Blueprint when the primary target dies.
	UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
	void PrimaryTargetDied(AActor* DeadActor);

	// Called from Blueprint when a chained target dies.
	UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
	void AdditionalTargetDied(AActor* DeadActor);

protected:
	// Location where the mouse hit the world.
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;

	// Actor that was hovered over by the mouse.
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	// Cached owner player controller.
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	// Cached owner character.
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	// Name of the weapon tip socket for beam origin.
	UPROPERTY(VisibleAnywhere, Category = "Beam", meta=(AllowPrivateAccess = "true"))
	FString WeaponTipSocket = "TipSocket";

	// Maximum number of additional targets the beam can chain to.
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShockTargets = 5;
};
