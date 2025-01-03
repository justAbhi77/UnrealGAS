//


#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Ensure the enemy mesh blocks visibility tracing for highlighting purposes
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Initialize Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Initialize Attribute Set
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Set custom depth stencil values for highlighting
	GetMesh()->SetCustomDepthStencilValue(HighlightValue);
	if(Weapon)
		Weapon->SetCustomDepthStencilValue(HighlightValue);

	// Initialize ability system info
	InitAbilityActorInfo();
}

#if WITH_EDITOR
void AAuraEnemy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Update weapon attachment if the weapon socket name changes in the editor
	if(PropertyName == GET_MEMBER_NAME_CHECKED(AAuraEnemy, WeaponSocket))
		if(Weapon && GetMesh())
			Weapon->SetupAttachment(GetMesh(), FName(WeaponSocket));
}
#endif

void AAuraEnemy::HighlightActor()
{
	// Enable custom depth rendering for mesh and weapon
	GetMesh()->SetRenderCustomDepth(true);
	if(Weapon)
		Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor()
{
	// Disable custom depth rendering for mesh and weapon
	GetMesh()->SetRenderCustomDepth(false);
	if(Weapon)
		Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	// Initialize ability system actor info for this enemy
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if(auto* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		AuraAbilitySystemComponent->AbilityActorInfoSet();
}

void AAuraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAuraEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
