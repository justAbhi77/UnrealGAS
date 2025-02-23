//

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

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

	// Clamp health and mana attributes after effects
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}

	if(Data.EvaluatedData.Attribute == GetManaAttribute())
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));

	// Handle incoming damage
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
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
					CombatInterface->Die();
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		}
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
