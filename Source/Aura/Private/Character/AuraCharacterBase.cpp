//


#include "Character/AuraCharacterBase.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Aura/Aura.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = GameplayTags.Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName(WeaponSocket));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionComponent");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);

	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);

	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBeingShocked);
}

void AAuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

float AAuraCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for(const FTaggedMontage& TaggedMontage : AttackMontages)
		if(TaggedMontage.MontageTag == MontageTag)
			return TaggedMontage;
	return FTaggedMontage();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
		return Weapon->GetSocketLocation(WeaponTipSocketName);

	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
		return GetMesh()->GetSocketLocation(LeftHandSocketName);

	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
		return GetMesh()->GetSocketLocation(RightHandSocketName);

	if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
		return GetMesh()->GetSocketLocation(TailSocketName);

	return FVector::ZeroVector;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	bDead = true;

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	BurnDebuffComponent->Deactivate();
	StunDebuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
}


void AAuraCharacterBase::OnRep_Stunned()
{
}

void AAuraCharacterBase::OnRep_Burned()
{
}

void AAuraCharacterBase::Dissolve()
{
	if(IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMat);
		StartDissolveTimeline(DynamicMat);
	}
	if(IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMat);
		StartWeaponDissolveTimeline(DynamicMat);
	}
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const
{
	if(!IsValid(AbilitySystemComponent) || !GameplayEffectClass) return;

	// Create and apply the gameplay effect
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	if(SpecHandle.IsValid())
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	// Apply default attributes at level 1
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if(!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

#if WITH_EDITOR
void AAuraCharacterBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Update weapon attachment if the socket name is modified
	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(PropertyName == GET_MEMBER_NAME_CHECKED(AAuraCharacterBase, WeaponSocket))
		if(Weapon && GetMesh())
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName(WeaponSocket));
}
#endif
