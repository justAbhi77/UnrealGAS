//


#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Math/MathFwd.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraGameplayTags.h"
#include "Ai/AuraAiController.h"
#include "Aura/AuraLogChannels.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Ensure enemy mesh blocks visibility tracing for highlighting
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAura, Display, TEXT("Begin Play for [%s]"), *GetNameSafe(this));

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	// Set custom depth stencil values for highlighting
	GetMesh()->SetCustomDepthStencilValue((int32)HighlightValue);
	if(Weapon) Weapon->SetCustomDepthStencilValue((int32)HighlightValue);

	// Initialize ability system
	InitAbilityActorInfo();

	if(HasAuthority())
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);

	// Setup UI widget and attribute change delegates
	if(auto* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
		AuraUserWidget->SetWidgetController(this);

	if(const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).
			AddLambda([this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); });

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute())
			.AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogAura, Display, TEXT("Enemy [%s] Possessed by [%s]"), *GetNameSafe(this), *GetNameSafe(NewController));

	if(!HasAuthority()) return;

	// Assign AI controller and initialize behavior tree
	AuraAIController = Cast<AAuraAiController>(NewController);
	if(AuraAIController && BehaviorTree)
	{
		UBlackboardComponent* BlackboardComp = AuraAIController->GetBlackboardComponent();
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AuraAIController->RunBehaviorTree(BehaviorTree);

		BlackboardComp->SetValueAsBool(FName("HitReacting"), false);
		BlackboardComp->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	}
}

#if WITH_EDITOR
void AAuraEnemy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Update weapon attachment if the weapon socket name changes in the editor
	if(PropertyName == GET_MEMBER_NAME_CHECKED(AAuraEnemy, WeaponSocket))
		if(Weapon && GetMesh())
			Weapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), WeaponSocket);
}
#endif

void AAuraEnemy::HighlightActor_Implementation()
{
	UE_LOG(LogAura, Display, TEXT("Highlight enabled for [%s]"), *GetNameSafe(this));
	// Enable custom depth rendering for mesh and weapon
	GetMesh()->SetRenderCustomDepth(true);
	if(Weapon) Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	UE_LOG(LogAura, Display, TEXT("Highlight Disabled for [%s]"), *GetNameSafe(this));
	// Disable custom depth rendering for mesh and weapon
	GetMesh()->SetRenderCustomDepth(false);
	if(Weapon) Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	// Initialize ability system for this enemy
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if(auto* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		AuraASC->AbilityActorInfoSet();

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::StunTagChanged);

	if(HasAuthority())
		InitializeDefaultAttributes();

	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	
	if(AuraAIController && AuraAIController->GetBlackboardComponent())
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
}

void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);

	if(AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);

	SpawnLoot();
	Super::Die(DeathImpulse);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if(AuraAIController)
		if(UBlackboardComponent* BlackboardComponent = AuraAIController->GetBlackboardComponent())
			BlackboardComponent->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AAuraEnemy::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	
}