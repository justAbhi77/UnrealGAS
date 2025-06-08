//


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
class UNiagaraSystem;
class USoundBase;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAscRegistered, UAbilitySystemComponent*)

/**
 * Struct representing an attack montage with associated tags and effects.
 */
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	// Animation montage for the attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	// GameplayTag associated with this montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	// Tag representing the socket used for this attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	// Sound effect played upon impact
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for combat-related functionality, providing a standardized way
 * to query combat-specific data and implement combat behavior in actors.
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	// Returns the player's level.
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel() const;

	/**
	 * Retrieves the socket location on the weapon based on the montage tag.
	 * @return A FVector representing the socket location.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	// Updates the actor's facing direction based on the target position.
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	// Returns the montage played when the actor gets hit.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	// Check if the actor is dead.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	// Returns the actor's avatar.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	// Handles the death logic for the actor.
	virtual void Die(const FVector& DeathImpulse) = 0;

	// Returns a list of attack montages available to the actor.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	// Returns the blood effect used when the actor takes damage.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	// Retrieves an attack montage based on its tag.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	// Returns the number of minions currently alive.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	// Increments the number of minions.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	// Returns the character class.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	virtual FOnAscRegistered GetOnAscRegisteredDelegate() = 0;
};
