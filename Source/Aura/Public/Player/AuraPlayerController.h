//


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class AAuraCharacter;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UNiagaraSystem;

/**
 * Handles player input, cursor interaction, auto-running, and combat-related ability inputs.
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// Input settings
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ScrollAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	// Ability system
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	// Input functions
	void Move(const FInputActionValue& InputActionValue);
	void ShiftPressed();
	void ShiftReleased();
	void ScrollMoved(const FInputActionValue& InputActionValue);

	// State flags
	bool bShiftKeyDown = false;
	bool bAutoRunning = false;
	bool bTargeting = false;

	// Cursor trace info
	FHitResult CursorHit;
	void CursorTrace();

	// Actor highlighting(Reference to Interfaces)
	TScriptInterface<IEnemyInterface> LastActor, ThisActor;

	// Ability input handling
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	// Auto-run settings
	UPROPERTY(EditDefaultsOnly, Category = "AutoRun")
	float AutoRunAcceptanceRadius = 50.f;

	// For auto-run navigation
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly, Category = "AutoRun")
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	// Movement cache
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	const float ShortPressThreshold = 0.5f;

	// Damage number display
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	// Cached reference to Aura character
	UPROPERTY()
	AAuraCharacter* AuraPawn = nullptr;
};
