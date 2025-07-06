//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

// Defines when an effect should be applied
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap, // Apply when overlapping begins
	ApplyOnEndOverlap, // Apply when overlapping ends
	DoNotApply // Never apply
};

// Defines when an effect should be removed
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap, // Remove when overlap ends
	DoNotRemove // Never remove
};

/**
 * Struct to hold effect class with its application and removal policy
 */
USTRUCT(BlueprintType)
struct FEffectToApply
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};

/**
 * Applies or removes gameplay effects based on overlap events.
 */
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bRotates = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float RotationRate = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bSinusoidalMovement = false;

	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();

	UFUNCTION(BlueprintCallable)
	void StartRotation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SineAmplitude = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SinePeriodConstant = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	FVector InitialLocation;
protected:
	virtual void BeginPlay() override;

	// Determines if the actor should be destroyed after applying or removing an effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = true;

	// If true, effects will also apply to enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	// Level scaling for applied effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.f;

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	TArray<FEffectToApply> EffectsToApply;

	// Map storing active effect handles associated with ability system components
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass, EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove);

	UFUNCTION()
	void RemoveEffectsFromTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

private:
	bool bCanDestroyOnEffectApplication = true, bCanDestroyOnEffectRemoval = false;

	float RunningTime = 0.f;
	void ItemMovement(float DeltaTime);
};
