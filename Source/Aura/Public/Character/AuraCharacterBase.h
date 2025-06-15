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
class UDebuffNiagaraComponent;

/**
 * Base class for characters with abilities and combat capabilities.
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override{ return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void Die(const FVector& DeathImpulse) override;
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
	virtual FOnAscRegistered& GetOnAscRegisteredDelegate() override{ return OnAscRegistered; }
	virtual FOnDeathSignature& GetOnDeathDelegate() override{ return OnDeathDelegate; }
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override{ return Weapon; }
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override{ bIsBeingShocked = bInShock; }
	virtual bool IsBeingShocked_Implementation() const override{ return bIsBeingShocked; }

	FOnAscRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

#if WITH_EDITOR
	// Handles property changes in the editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

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

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;

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

	UPROPERTY(VisibleAnywhere, Category = "Debuff")
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere, Category = "Debuff")
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
};
