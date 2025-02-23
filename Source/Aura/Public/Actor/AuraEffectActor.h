// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

// Defines when effects should be applied
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap, // Effect applied when overlapping starts
	ApplyOnEndOverlap, // Effect applied when overlap ends
	DoNotApply // Effect not applied
};

// Defines when effects should be removed
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap, // Effect removed when overlap ends
	DoNotRemove // Effect not removed
};

// Struct defining an effect with its application and removal policies
USTRUCT(BlueprintType)
struct FEffectToApply
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffectClass; // Effect class to apply

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;

protected:
	virtual void BeginPlay() override;

	// Determines if the actor should be destroyed after applying or removing an effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectApplication = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectRemoval = true;

	// If true, effects will also apply to enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bApplyEffectsToEnemies = false;

	// Level scaling for applied effects
	UPROPERTY(EditAnywhere, Category="Applied Effects")
	float ActorLevel = 1.f;

	// List of effects to apply
	UPROPERTY(EditAnywhere, Category="Applied Effects")
	TArray<FEffectToApply> EffectsToApply;

	// Map storing active effect handles associated with ability system components
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	// Applies an effect to a target actor
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass, EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove);

	// Removes all applied effects from a target actor
	UFUNCTION()
	void RemoveEffectsFromTarget(AActor* TargetActor);

	// Handles actor overlap events
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// Handles actor end-overlap events
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

private:
	bool bCanDestroyOnEffectApplication = true, bCanDestroyOnEffectRemoval = false;
};
