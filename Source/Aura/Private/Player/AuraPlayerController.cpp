//


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	// Initialize spline for auto-run navigation
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Handle cursor tracing and auto-run
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	// Manage highlighting and unhighlighting of actors
	if(LastActor != ThisActor)
	{
		if(LastActor) LastActor->UnHighlightActor();
		if(ThisActor) ThisActor->HighlightActor();
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

void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning) return;

	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		// ReSharper disable once CppTooWideScopeInitStatement
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
			bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// If the input tag is not matched, release the ability input tag and return early.
		if(GetASC())
			GetASC()->AbilityInputTagReleased(InputTag);

		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if (!bTargeting && !bShiftKeyDown)
	{
		// If not targeting, get the controlled pawn and process the pathfinding logic.
		const APawn* ControlledPawn = GetPawn();
		if(ControlledPawn && FollowTime <= ShortPressThreshold)
		{
			// Find a path to the cached destination using the navigation system.
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				// Clear the current spline path and rebuild it using the calculated navigation path.
				Spline->ClearSplinePoints();
				for(const auto& PointLoc : NavPath->PathPoints)
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);

				// Ensure the last point on the path is the one the character should move towards,
				// in case the destination point is unreachable due to nav mesh modifiers(blocking vol, etc.).
				CachedDestination = NavPath->PathPoints.Last();

				bAutoRunning = NavPath->PathPoints.Num() != 0; // Mark the character for auto-running.
			}
		}

		// Reset data since input released 
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// If input tag does not match, hold the ability input tag and return early.
		if(GetASC())
			GetASC()->AbilityInputTagHeld(InputTag);

		return;
	}

	if(bTargeting || bShiftKeyDown)
	{
		// If targeting is active, continue holding the input tag.
		if(GetASC())
			GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// Update follow time.
		FollowTime += GetWorld()->GetDeltaSeconds();

		// If a blocking hit occurred during the trace, update the cached destination with the hit location.
		// ReSharper disable once CppTooWideScopeInitStatement
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

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(!AuraAbilitySystemComponent)
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation(), YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X),
	RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
