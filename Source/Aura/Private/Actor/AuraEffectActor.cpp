//


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/KismetMathLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// Skip if target is an enemy and effects shouldn't be applied to enemies
	if(TargetActor->Implements<UEnemyInterface>() && !bApplyEffectsToEnemies) return;

	UE_LOG(LogAura, Display, TEXT("Applying Begin Overlap effects to actor [%s] from [%hs]"), *GetNameSafe(TargetActor) ,__FUNCTION__);

	// Apply effects that should trigger on overlap
	for(const auto& EffectToApply : EffectsToApply)
		if(EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
			ApplyEffectToTarget(TargetActor, EffectToApply.GameplayEffectClass, EffectToApply.EffectRemovalPolicy);

	// Destroy self if required
	if(bCanDestroyOnEffectApplication && bDestroyOnEffectApplication)
		Destroy();
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(TargetActor->Implements<UEnemyInterface>() && !bApplyEffectsToEnemies) return;

	UE_LOG(LogAura, Display, TEXT("Applying End Overlap effects to actor [%s] from [%hs]"), *GetNameSafe(TargetActor) ,__FUNCTION__);

	for(const auto& EffectToApply : EffectsToApply)
		if(EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
			ApplyEffectToTarget(TargetActor, EffectToApply.GameplayEffectClass, EffectToApply.EffectRemovalPolicy);

	// Remove applied effects from target
	RemoveEffectsFromTarget(TargetActor);

	// Destroy the actor if required
	if(bCanDestroyOnEffectRemoval && bDestroyOnEffectRemoval)
	{
		UE_LOG(LogAura, Display, TEXT("Destroying self after applying effects from [%hs]"), __FUNCTION__);
		Destroy();
	}
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass, EEffectRemovalPolicy RemovalPolicy)
{
	if(!GameplayEffectClass) return;

	// Skip if target is an enemy and effects shouldn't be applied to enemies
	if(TargetActor->Implements<UEnemyInterface>() && !bApplyEffectsToEnemies) return;

	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

		// Check if effect is infinite and should be removed on end overlap
		const bool bInfinite = GameplayEffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if(bInfinite && RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
			bCanDestroyOnEffectApplication = false;
		}

		/* Destroy the actor if the current effect is not infinite
		 *if(!bInfinite)
			Destroy();
		*/
	}
}

void AAuraEffectActor::RemoveEffectsFromTarget(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!IsValid(TargetASC)) return;

	TArray<FActiveGameplayEffectHandle> HandlesToRemove;

	// Cache effects applied to this actor that should be removed
	for(const auto& HandlePair : ActiveEffectHandles)
		if(TargetASC == HandlePair.Value)
		{
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
			HandlesToRemove.Add(HandlePair.Key);
		}

	// Remove handles
	for(const auto& Handle : HandlesToRemove)
		ActiveEffectHandles.Remove(Handle);

	if(ActiveEffectHandles.Num() == 0)
		bCanDestroyOnEffectRemoval = true;
}

void AAuraEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	const float SinePeriod = 2 * PI / SinePeriodConstant;

	if(RunningTime > SinePeriod)
		RunningTime = 0.f;

	ItemMovement(DeltaTime);
}

void AAuraEffectActor::ItemMovement(float DeltaTime)
{
	if(bRotates)
	{
		const FRotator DeltaRotation(0.f, DeltaTime * RotationRate, 0.f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}

	if(bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
}

void AAuraEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

void AAuraEffectActor::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
}
