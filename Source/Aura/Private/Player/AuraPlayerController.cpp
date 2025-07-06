//


#include "Player/AuraPlayerController.h"
#include "Engine/EngineTypes.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Character/AuraCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Aura/Aura.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(!AuraContext) return;

	UE_LOG(LogAura, Display, TEXT("PlayerController Initialized"));

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace(); // Handle cursor tracing and auto-run
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::CursorTrace()
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);

		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if(IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
		ThisActor = CursorHit.GetActor();
	else
		ThisActor = nullptr;

	// Handle actor highlighting
	if(LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}

	/*
	* A. line trace from cursor, all cases
	* B. lastActor null and thisActor null - do nothing
	* C. lastActor null and thisActor valid - highlight thisActor
	* D. lastActor valid and thisActor null - unhighlight lastActor
	* E. both valid but lastActor != ThisActor - unhighlight lastActor and highlight thisActor
	* F. both valid but lastActor == ThisActor - do nothing
	*/

	/*
	 if(!LastActor)
	{
		if(ThisActor)
		{
			// case B
			ThisActor->HighlightActor();
		}
		else
		{
			// case A
		}
	}
	else
	{
		// Valid Last Actor
		if(!ThisActor)
		{
			// case C
			LastActor->UnHighlightActor();
		}
		else
		{
			// both actors valid
			if(LastActor != ThisActor)
			{
				// case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// case E
			}
		}
	}
	*/
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if(IsValid(InActor) && InActor->Implements<UHighlightInterface>())
		IHighlightInterface::Execute_HighlightActor(InActor);
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if(IsValid(InActor) && InActor->Implements<UHighlightInterface>())
		IHighlightInterface::Execute_UnHighlightActor(InActor);
}

void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning) return;

	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		// ReSharper disable once CppTooWideScopeInitStatement
		const float DistanceToDestination = FVector::Dist(LocationOnSpline, CachedDestination);
		if(FMath::IsNearlyZero(DistanceToDestination, AutoRunAcceptanceRadius))
		{
			UE_LOG(LogAura, Display, TEXT("Auto Running Completed"));
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		return;

	UE_LOG(LogAura, Display, TEXT("Input Pressed [%s]"), *InputTag.ToString());

	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(IsValid(ThisActor))
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		else
			TargetingStatus = ETargetingStatus::NotTargeting;
		
		bAutoRunning = false;
	}

	if(GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
		return;

	if(GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) return;

	if(TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		// If not targeting, get the controlled pawn and process the pathfinding logic.
		if(const APawn* ControlledPawn = GetPawn())
			if(ControlledPawn && FollowTime <= ShortPressThreshold)
			{
				if(IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
					IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
				else if(GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);

				// Find a path to the cached destination using the navigation system.
				if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
				{
					// Clear the current spline path and rebuild it using the calculated navigation path.
					Spline->ClearSplinePoints();
					for(const auto& PointLoc : NavPath->PathPoints)
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);

					// Ensure the last point on the path is the one the character should move towards,
					// in case the destination point is unreachable due to nav mesh modifiers(blocking vol, etc.).
					if(!NavPath->PathPoints.IsEmpty())
						CachedDestination = NavPath->PathPoints.Last();

					bAutoRunning = !NavPath->PathPoints.IsEmpty(); // Mark the character for auto-running.
				}
			}

		// Reset data since input released
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
		return;

	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if(TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		if(GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// Update follow time.
		FollowTime += GetWorld()->GetDeltaSeconds();

		// If a blocking hit occurred during the trace, update the cached destination with the hit location.
		if(CursorHit.bBlockingHit)
			CachedDestination = CursorHit.Location; // Alternatively, ImpactPoint can be used if the trace method differs (e.g., sphere trace).

		if(APawn* ControlledPawn = GetPawn())
		{
			// Calculate the direction from the pawn's current location to the cached destination.
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if(!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if(DecalMaterial)
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if(IsValid(MagicCircle))
		MagicCircle->Destroy();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	AuraInputComponent->BindAction(ScrollAction, ETriggerEvent::Started, this, &ThisClass::ScrollMoved);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
		return;

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation(), YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
		if(bAutoRunning)
			UE_LOG(LogAura, Display, TEXT("Auto Running Canceled by movement input"));
		bAutoRunning = false;
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if(IsValid(MagicCircle))
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
}

inline void AAuraPlayerController::ShiftPressed()
{
	UE_LOG(LogAura, Display, TEXT("Shift Key Pressed"));
	bShiftKeyDown = true;
}

inline void AAuraPlayerController::ShiftReleased()
{
	UE_LOG(LogAura, Display, TEXT("Shift Key Released"));
	bShiftKeyDown = false;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void AAuraPlayerController::ScrollMoved(const FInputActionValue& InputActionValue)
{
	const float InputAxisValue = InputActionValue.Get<float>();

	UE_LOG(LogAura, Display, TEXT("Scroll Wheel moved"));

	if(!AuraPawn) AuraPawn = GetPawn<AAuraCharacter>();
	if(AuraPawn) AuraPawn->MoveSpringArm(InputAxisValue);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	return AuraAbilitySystemComponent;
}
