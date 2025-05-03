//


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAnimMontage;
class UNiagaraSystem;

/**
 * Base class for characters with abilities and combat capabilities.
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override{ return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void Die() override;
	virtual bool IsDead_Implementation() const override { return bDead; }
	virtual AActor* GetAvatar_Implementation() override{ return this; }
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override{ return AttackMontages; }
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override{ return BloodEffect; }
	virtual UAnimMontage* GetHitReactMontage_Implementation() override{ return HitReactMontage; }
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override{ return MinionCount; }
	virtual void IncrementMinionCount_Implementation(int32 Amount) override{ MinionCount += Amount; }
	virtual ECharacterClass GetCharacterClass_Implementation() override{ return CharacterClass; }

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

#if WITH_EDITOR
	// Handles property changes in the editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	bool bDead = false;
	int32 MinionCount = 0;

	// Initializes the actor info for the ability system
	virtual void InitAbilityActorInfo();

	// Initialize default attributes
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	// Applies a specified effect to self
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	void Dissolve();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// Name of the socket where the weapon is attached
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSocket = "WeaponHandSocket";

	// Socket name for the tip of the weapon
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	// Name of the socket where the Left hand is
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	// Name of the socket where the right hand is
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	// Name of the socket where the tail hand is
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// AttributeSet containing the character's attributes
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// Character's primary attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// Character's secondary attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// Character's vital attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Character's abilities at the start
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// Character's passive abilities at the start
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
};
