// 


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// Skip if target is an enemy and effects shouldn't be applied to enemies
	if(TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	// Apply effects that should trigger on overlap
	for(const auto& EffectToApply : EffectsToApply)
		if(EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
			ApplyEffectToTarget(TargetActor, EffectToApply.GameplayEffectClass, EffectToApply.EffectRemovalPolicy);

	// Destroy the actor if required
	if(bCanDestroyOnEffectApplication && bDestroyOnEffectApplication)
		Destroy();
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

	// Apply effects that should trigger when overlap ends
	for(const auto& EffectToApply : EffectsToApply)
		if(EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
			ApplyEffectToTarget(TargetActor, EffectToApply.GameplayEffectClass, EffectToApply.EffectRemovalPolicy);

	// Remove applied effects from target
	RemoveEffectsFromTarget(TargetActor);

	// Destroy the actor if required
	if(bCanDestroyOnEffectRemoval && bDestroyOnEffectRemoval)
		Destroy();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass, EEffectRemovalPolicy RemovalPolicy)
{
	if(!GameplayEffectClass) return;

	// Skip if target is an enemy and effects shouldn't be applied to enemies
	if(TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;

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
