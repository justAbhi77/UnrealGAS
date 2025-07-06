//

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "HAL/Platform.h"
#include "Interaction/CombatInterface.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Math/MathFwd.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Aura/AuraLogChannels.h"
#include "AuraAbilityTypes.h"
#include "Interaction/PlayerInterface.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Initialize Primary Attributes
	TagsToAttributes = {
		{GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute},
		{GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute},
		{GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute},
		{GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute},

		// Initialize Secondary Attributes
		{GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute},
		{GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute},
		{GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute},
		{GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute},
		{GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute},
		{GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute},
		{GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute},
		{GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute},
		{GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute},
		{GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute},

		// Initialize Resistance Attributes
		{GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute},
		{GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute},
		{GameplayTags.Attributes_Resistance_Lighting, GetLightningResistanceAttribute},
		{GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute}
	};
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Macro to simplify replication declaration
	#define REGISTER_REPLICATED_ATTRIBUTE(Attribute) \
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attribute, COND_None, REPNOTIFY_Always)

	// Primary Attributes
	REGISTER_REPLICATED_ATTRIBUTE(Strength);
	REGISTER_REPLICATED_ATTRIBUTE(Intelligence);
	REGISTER_REPLICATED_ATTRIBUTE(Resilience);
	REGISTER_REPLICATED_ATTRIBUTE(Vigor);

	// Secondary Attributes
	REGISTER_REPLICATED_ATTRIBUTE(Armor);
	REGISTER_REPLICATED_ATTRIBUTE(ArmorPenetration);
	REGISTER_REPLICATED_ATTRIBUTE(BlockChance);
	REGISTER_REPLICATED_ATTRIBUTE(CriticalHitChance);
	REGISTER_REPLICATED_ATTRIBUTE(CriticalHitDamage);
	REGISTER_REPLICATED_ATTRIBUTE(CriticalHitResistance);
	REGISTER_REPLICATED_ATTRIBUTE(HealthRegeneration);
	REGISTER_REPLICATED_ATTRIBUTE(ManaRegeneration);
	REGISTER_REPLICATED_ATTRIBUTE(MaxHealth);
	REGISTER_REPLICATED_ATTRIBUTE(MaxMana);

	// Vital Attributes
	REGISTER_REPLICATED_ATTRIBUTE(Health);
	REGISTER_REPLICATED_ATTRIBUTE(Mana);

	// Resistance Attributes
	REGISTER_REPLICATED_ATTRIBUTE(ArcaneResistance);
	REGISTER_REPLICATED_ATTRIBUTE(FireResistance);
	REGISTER_REPLICATED_ATTRIBUTE(LightningResistance);
	REGISTER_REPLICATED_ATTRIBUTE(PhysicalResistance);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp health and mana attributes before applying changes
	if(Attribute == GetHealthAttribute()) NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());

	if(Attribute == GetManaAttribute()) NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

	// Clamp health and mana attributes after effects
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

	if(Data.EvaluatedData.Attribute == GetManaAttribute())
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));

	// Handle incoming damage
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
		HandleIncomingDamage(Props);

	if(Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
		HandleIncomingXp(Props);
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if(Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this Ability Sped)
    // Extract source and target properties from the gameplay effect context
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() &&
		Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if(!Props.SourceController && Props.SourceAvatarActor)
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
				Props.SourceController = Pawn->GetController();

		if(Props.SourceController)
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if(!IsValid(Props.SourceCharacter) || !IsValid(Props.TargetCharacter)) return;
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if(AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if(AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
	}

	const FString PrintMessage = FString::Printf(TEXT("blocked: %hs critical: %hs"), bBlockedHit? "true" : "false", bCriticalHit? "true" : "false");
	UKismetSystemLibrary::PrintString(this, PrintMessage);
}

void UAuraAttributeSet::SendXpEvent(const FEffectProperties& Props)
{
	if(Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXpRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXp;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXp, Payload);
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0);
	if(LocalIncomingDamage > 0)
	{
		const float NewHealth = FMath::Clamp(GetHealth() - LocalIncomingDamage, 0.f, GetMaxHealth());
		SetHealth(NewHealth);

		if(NewHealth <= 0.f)
		{
			if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));

			SendXpEvent(Props);
		}
		else
		{
			if(Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if(!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, false, false); // XY override, Z override true for full force knockback false for counteract
				// (I wanted this so enemies standing still receive the full launch force, while those running towards Aura counteract that force).
				// Thank you Diego for the suggestion!
				
				Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
			}
		}

		const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		
		if(UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
			Debuff(Props);
	}
}

void UAuraAttributeSet::HandleIncomingXp(const FEffectProperties& Props)
{
	const float LocalIncomingXp = GetIncomingXp();
	SetIncomingXp(0);

	if(Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXp = IPlayerInterface::Execute_GetXp(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXp(Props.SourceCharacter, CurrentXp + LocalIncomingXp);

		const int32 DeltaLevelUps = NewLevel - CurrentLevel;

		if(DeltaLevelUps > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, DeltaLevelUps);

			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			for(int32 i = 0; i < DeltaLevelUps; ++i)
			{
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
			}

			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}

		IPlayerInterface::Execute_AddToXp(Props.SourceCharacter, LocalIncomingXp);
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	// Set the effect to not be applied immediately but after the first period
	Effect->bExecutePeriodicEffectOnApplication = false;

	// Deprecated as of 5.3.2
	// Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuff[DamageType]);
	/**
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
	Effect->InheritableOwnedTagsContainer.AddTag(DebuffTag);
	if(DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
	{
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_CursorTrace);
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputHeld);
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputPressed);
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	*/
	// Workaround
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuff[DamageType];

	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(DebuffTag);

	if(DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
	{
		TagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}

	Component.SetAndApplyTargetTagChanges(TagContainer);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	// make it non const to be used for multiple fGameplayModifiers
	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();

	if(FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}
