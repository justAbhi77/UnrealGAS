//

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);

/**
 * Implements ability system interface and handles player attributes and levels.
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Returns the associated Ability System Component
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Returns the associated Attribute Set
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Returns the player's current level
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	// Returns the player's current Experience
	FORCEINLINE int32 GetPlayerExp() const { return Exp; }

	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }

	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InPoints);
	void AddToSpellPoints(int32 InPoints);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

protected:
	// Ability System Component for managing abilities
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attribute Set containing player stats and attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	// Player level, replicated to all clients
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "PlayerState")
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	// Player Experience, replicated to all clients
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Exp, Category = "PlayerState")
	int32 Exp = 1;

	UFUNCTION()
	void OnRep_Exp(int32 OldExp);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints, Category = "PlayerState")
	int32 AttributePoints = 0;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints, Category = "PlayerState")
	int32 SpellPoints = 1;

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
