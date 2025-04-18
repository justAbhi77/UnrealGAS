//

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

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
	virtual int32 GetPlayerLevel() const override;

	// 
	virtual void AddToXp_Implementation(int32 InXp) override;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Blueprintable, BlueprintImplementableEvent)
	void MoveSpringArm(float Value);
};
