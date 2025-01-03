// 


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize the root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for(const auto& EffectToApply : EffectsToApply)
		if(EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
			ApplyEffectToTarget(TargetActor, EffectToApply.GameplayEffectClass, EffectToApply.EffectRemovalPolicy);

	if(bCanDestroyOnEffectApplication && bDestroyOnEffectApplication)
		Destroy();
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for(const auto& EffectToApply : EffectsToApply)
		if(EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
			ApplyEffectToTarget(TargetActor, EffectToApply.GameplayEffectClass, EffectToApply.EffectRemovalPolicy);

	RemoveEffectsFromTarget(TargetActor);

	if(bCanDestroyOnEffectRemoval && bDestroyOnEffectRemoval)
		Destroy();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass,
	EEffectRemovalPolicy RemovalPolicy)
{
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

		if(GameplayEffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite &&
			RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
			bCanDestroyOnEffectApplication = false;
		}
	}
}

void AAuraEffectActor::RemoveEffectsFromTarget(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!IsValid(TargetASC)) return;

	TArray<FActiveGameplayEffectHandle> HandlesToRemove;
	for(const auto& HandlePair : ActiveEffectHandles)
		if(TargetASC == HandlePair.Value)
		{
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
			HandlesToRemove.Add(HandlePair.Key);
		}

	for(const auto& Handle : HandlesToRemove)
		ActiveEffectHandles.Remove(Handle);

	if(ActiveEffectHandles.Num() == 0)
		bCanDestroyOnEffectRemoval = true;
}
