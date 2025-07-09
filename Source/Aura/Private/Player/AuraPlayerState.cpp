//


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/AuraLogChannels.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerLevel);
	DOREPLIFETIME(ThisClass, Exp);
	DOREPLIFETIME(ThisClass, AttributePoints);
	DOREPLIFETIME(ThisClass, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToXp(int32 InXp)
{
	Exp += InXp;
	UE_LOG(LogAura, Display, TEXT("Player received %d Experience points"), InXp);
	OnXPChangedDelegate.Broadcast(Exp);
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	PlayerLevel += InLevel;
	UE_LOG(LogAura, Display, TEXT("Player leveled up by %d"), InLevel);
	OnLevelChangedDelegate.Broadcast(PlayerLevel, true);
}

void AAuraPlayerState::AddToAttributePoints(const int32 InPoints)
{
	AttributePoints += InPoints;
	UE_LOG(LogAura, Display, TEXT("Player received %d Attribute points"), InPoints);
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	UE_LOG(LogAura, Display, TEXT("Player received %d Spell points"), InPoints);
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	Exp = InXP;
	OnXPChangedDelegate.Broadcast(Exp);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	PlayerLevel = InLevel;
	OnLevelChangedDelegate.Broadcast(PlayerLevel, false);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel) const
{
	UE_LOG(LogAura, Display, TEXT("Player leveled up by %d"), PlayerLevel);
	OnLevelChangedDelegate.Broadcast(PlayerLevel, true);
}

void AAuraPlayerState::OnRep_Exp(int32 OldExp) const
{
	UE_LOG(LogAura, Display, TEXT("Player received %d Experience points"), Exp);
	OnXPChangedDelegate.Broadcast(Exp);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints) const
{
	UE_LOG(LogAura, Display, TEXT("Player received %d Attribute points"), AttributePoints);
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints) const
{
	UE_LOG(LogAura, Display, TEXT("Player received %d Spell points"), SpellPoints);
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
