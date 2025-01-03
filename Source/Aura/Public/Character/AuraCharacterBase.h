//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

/**
 * Base class for characters with abilities and combat capabilities.
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	// Returns the Ability System Component of the character
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

#if WITH_EDITOR
	// Handles property changes in the editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay() override;

	// Skeletal mesh component for the character's weapon
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// Name of the socket where the weapon is attached
	UPROPERTY(EditAnywhere, Category = "Combat")
	FString WeaponSocket = "WeaponHandSocket";

	// Socket name for the tip of the weapon
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	// Ability System Component responsible for managing abilities
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// AttributeSet containing the character's attributes
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// Initializes the actor info for the ability system
	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Applies a specified effect to self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	// Initialize default attributes
	void InitializeDefaultAttributes() const;

	// Add abilities to self
	void AddCharacterAbilities();

	// Return the location of the weapon tip socket
	virtual FVector GetCombatSocketLocation() override;

private:
	// Startup Abilities of the character 
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
