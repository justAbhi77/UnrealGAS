//


#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class AAuraPlayerState;
class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * Player-controlled character class for Aura.
 * Extends AAuraCharacterBase and implements player-specific functionality through IPlayerInterface.
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

	virtual void BeginPlay() override;

	// Called when the character is possessed by a new controller (server only)
	virtual void PossessedBy(AController* NewController) override;

	// Called when PlayerState is replicated (client only)
	virtual void OnRep_PlayerState() override;

	// Initializes ability system actor info
	virtual void InitAbilityActorInfo() override;

	// Plays level-up VFX
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	// spring arm Target Length
	void MoveSpringArm(float Value);

	// Niagara effect component for level-up VFX
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void Tick(float DeltaSeconds) override;

protected:
	// Spring arm that controls the camera position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	// Top-down camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float DesiredTargetArmLength = 700;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float ScrollMultiplier = 150;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float CameraZoomInterpSpeed = 2;

	UPROPERTY()
	AAuraPlayerState* AuraPlayerState;

	// Interface overrides
	virtual int32 GetPlayerLevel_Implementation() const override;
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
};
