﻿//


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "TimerManager.h"
#include "UI/HUD/AuraHUD.h"
#include "NiagaraComponent.h"
#include "Aura/AuraLogChannels.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0, 400, 0);
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Aura is Elementalist by Design
	CharacterClass = ECharacterClass::Elementalist;
}

#if WITH_EDITOR
void AAuraCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(PropertyName == GET_MEMBER_NAME_CHECKED(AAuraCharacter, WeaponSocket))
		if(Weapon && GetMesh())
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
}
#endif

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAura, Display, TEXT("Begin Play for [%s]"), *GetNameSafe(this));

	AuraPlayerState = GetPlayerState<AAuraPlayerState>();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	if(!AuraPlayerState) return;

	UAbilitySystemComponent* SystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	SystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(SystemComponent))
		AuraASC->AbilityActorInfoSet();

	AbilitySystemComponent = SystemComponent;
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	if(auto* AuraPC = Cast<AAuraPlayerController>(GetController()))
		if(auto* AuraHUD = Cast<AAuraHUD>(AuraPC->GetHUD()))
			AuraHUD->InitOverlay(AuraPC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
}

void AAuraCharacter::MoveSpringArm(float Value)
{
	if(!CameraBoom) return;
	DesiredTargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + (Value * ScrollMultiplier), 200.f, 1000.f);
}

void AAuraCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!CameraBoom) return;
	const float CurrentArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DesiredTargetArmLength, DeltaSeconds, CameraZoomInterpSpeed);
	CameraBoom->TargetArmLength = CurrentArmLength;
}

void AAuraCharacter::OnRep_Stunned()
{
	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if(bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if(bIsBurned) BurnDebuffComponent->Activate();
	else BurnDebuffComponent->Deactivate();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogAura, Display, TEXT("Player [%s] possessed by [%s]"), *GetNameSafe(this), *GetNameSafe(NewController));

	InitAbilityActorInfo();

	LoadProgress();

	if(AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		AuraGameMode->LoadWorldState(GetWorld());
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UE_LOG(LogAura, Display, TEXT("PlayerState [%s] replicated for [%s]"), *GetNameSafe(GetPlayerState()), *GetNameSafe(this));
	InitAbilityActorInfo();
}

// Interface implementations

int32 AAuraCharacter::GetPlayerLevel_Implementation() const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState) return AuraPlayerState->GetPlayerLevel();
	return 0;
}

void AAuraCharacter::AddToXp_Implementation(int32 InXp)
{
	if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState) AuraPlayerState->AddToXp(InXp);
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState) AuraPlayerState->AddToLevel(InPlayerLevel);

	if(UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
		AuraAsc->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState) AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::AddToAttributePoints_Implementation(const int32 InAttributePoints)
{
	if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState) AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetXp_Implementation() const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState)	return AuraPlayerState->GetPlayerExp();
	return 0;
}

int32 AAuraCharacter::FindLevelForXp_Implementation(int32 InXp) const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState)	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXp);
	return 0;
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState)	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
	return 0;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState)	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;;
	return 0;
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState)	return AuraPlayerState->GetAttributePoints();
	return 0;
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	//if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	if(AuraPlayerState)	return AuraPlayerState->GetSpellPoints();
	return 0;
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(AuraGameMode)
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckpointTag;

		if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();
		if(AuraPlayerState)
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetPlayerExp();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false;

		if(!HasAuthority()) return;

		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = AuraASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;

			SaveData->SavedAbilities.AddUnique(SavedAbility);

		});
		AuraASC->ForEachAbility(SaveAbilityDelegate);

		AuraGameMode->SaveInGameProgressData(SaveData);
	}
}

void AAuraCharacter::LoadProgress()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(AuraGameMode)
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if(SaveData == nullptr) return;

		if(SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveData);

			if(!AuraPlayerState) AuraPlayerState = GetPlayerState<AAuraPlayerState>();
			if(AuraPlayerState)
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}

			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if(AuraGM)
			AuraGM->PlayerDied(this);
	});
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}
