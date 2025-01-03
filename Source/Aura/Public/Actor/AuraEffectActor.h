// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * Policy for applying effects
 */
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,	// Apply effect on overlap
	ApplyOnEndOverlap,	// Apply effect on end overlap
	DoNotApply	// Do not apply effect
};

/**
 * Policy for removing effects
 */
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,	// Remove effect on end overlap
	DoNotRemove	// Do not remove effect
};

/**
 * Struct defining an effect to apply
 */
USTRUCT(BlueprintType)
struct FEffectToApply
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;	// Effect class to apply

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

	// Effect application settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectApplication = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectRemoval = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.f;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	TArray<FEffectToApply> EffectsToApply;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	// Apply an effect to a target actor
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass,
		EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove);

	// Remove all applied effects from a target actor
	UFUNCTION()
	void RemoveEffectsFromTarget(AActor* TargetActor);

	// Handle overlap with another actor
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// Handle end of overlap with another actor
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

private:
	bool bCanDestroyOnEffectApplication = true, bCanDestroyOnEffectRemoval = false;
};
