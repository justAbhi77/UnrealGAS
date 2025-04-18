//


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Configure character movement
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0, 400, 0);
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->bSnapToPlaneAtStart = true;

	// Disable controller-based rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

#if WITH_EDITOR
void AAuraCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(PropertyName == GET_MEMBER_NAME_CHECKED(AAuraCharacter, WeaponSocket))
		if(Weapon && GetMesh())
			Weapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName(WeaponSocket));
}
#endif

void AAuraCharacter::InitAbilityActorInfo()
{
	// Retrieve player state and ensure it's valid
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if(!AuraPlayerState) return;

	// Initialize Ability System Component
	UAbilitySystemComponent* SystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	SystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	// Notify Ability System Component
	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(SystemComponent))
		AuraASC->AbilityActorInfoSet();

	AbilitySystemComponent = SystemComponent;
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// Initialize HUD overlay
	if(auto* AuraPC = Cast<AAuraPlayerController>(GetController()))
		if(auto* AuraHUD = Cast<AAuraHUD>(AuraPC->GetHUD()))
			AuraHUD->InitOverlay(AuraPC, AuraPlayerState, AbilitySystemComponent, AttributeSet);

	InitializeDefaultAttributes();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize ability system and grant abilities
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize ability system when PlayerState is replicated
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel() const
{
	// Retrieve player level from PlayerState
	if(const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
		return AuraPlayerState->GetPlayerLevel();

	return 0;
}

void AAuraCharacter::AddToXp_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAuraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
