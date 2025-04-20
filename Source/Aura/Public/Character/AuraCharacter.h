//

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * Extends AAuraCharacterBase to include player-specific functionality.
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()


public:
	AAuraCharacter();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Initializes character when possessed by a controller
	virtual void PossessedBy(AController* NewController) override;

	// Called when PlayerState is replicated to the client
	virtual void OnRep_PlayerState() override;

	// Returns the player’s current level
	virtual int32 GetPlayerLevel_Implementation() const override;

	//
	virtual void AddToXp_Implementation(int32 InXp) override;

	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;

	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;

	virtual void LevelUp_Implementation() override;

	virtual int32 GetXp_Implementation() const override;

	virtual int32 FindLevelForXp_Implementation(int32 InXp) const override;

	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;

	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;

	virtual int32 GetAttributePoints_Implementation() const override;

	virtual int32 GetSpellPoints_Implementation() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Blueprintable, BlueprintImplementableEvent)
	void MoveSpringArm(float Value);
};
