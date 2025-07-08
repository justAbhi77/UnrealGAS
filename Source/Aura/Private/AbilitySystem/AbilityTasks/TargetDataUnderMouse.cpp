// 


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Aura/Aura.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
}

void UTargetDataUnderMouse::Activate()
{
	if(Ability->GetCurrentActorInfo()->IsLocallyControlled())
		SendMouseCursorData();
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		// Register delegate for target data replication
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).
			AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// Check if data has already been replicated and call delegates if available
		if(!AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey))
			SetWaitingOnRemotePlayerData();
	}
}

void UTargetDataUnderMouse::SendMouseCursorData() const
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if(!PC) return;

	FHitResult CursorHit;
	bool HitResult = PC->GetHitResultUnderCursor(ECC_TARGET, false, CursorHit);
	if(!HitResult)
	{
		// If no hit under cursor, apply fallback
		FVector WorldLocation, WorldDirection;
		if(PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			FVector TraceStart = WorldLocation;
			FVector TraceEnd = TraceStart + WorldDirection * 1000.f; // 1,000 units forward

			// set location far in the direction
			CursorHit.Location = TraceEnd;
			CursorHit.ImpactPoint = TraceEnd;
			CursorHit.TraceEnd = TraceEnd;
			CursorHit.bBlockingHit = false;
		}
	}

	// Create gameplay target data
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// Send target data to the server with prediction support
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	// Broadcast valid target data if applicable
	if(ShouldBroadcastAbilityTaskDelegates())
		ValidData.Broadcast(DataHandle);
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if(ShouldBroadcastAbilityTaskDelegates())
		ValidData.Broadcast(DataHandle);
}
