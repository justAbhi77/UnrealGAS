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
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UNiagaraSystem;
class AMagicCircle;
class IHighlightInterface;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

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

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle() const;

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

	UAuraAbilitySystemComponent* GetAsc();

	// Input functions
	void Move(const FInputActionValue& InputActionValue);
	void ShiftPressed();
	void ShiftReleased();
	void ScrollMoved(const FInputActionValue& InputActionValue);

	// State flags
	bool bShiftKeyDown = false;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	// Cursor trace info
	FHitResult CursorHit;
	void CursorTrace();

	// Actor highlighting
	UPROPERTY()
	TObjectPtr<AActor> LastActor;

	UPROPERTY()
	TObjectPtr<AActor> ThisActor;

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation() const;
};
