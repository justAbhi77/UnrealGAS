// 

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

// Macro to generate standard accessor functions for attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Macro to generate OnRep function for @PropertyName
#define ON_REP_IMPL(PropertyName) \
void OnRep_##PropertyName(const FGameplayAttributeData& OldValue) const { \
GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PropertyName, OldValue); \
}

// Macro to generate Attribute (let compiler do the heavy lifting :))
#define ATTRIBUTE_PROPERTY(PropertyName) \
UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_##PropertyName, Category = "Attributes") \
FGameplayAttributeData PropertyName; \
ATTRIBUTE_ACCESSORS(ThisClass, PropertyName); \
UFUNCTION() ON_REP_IMPL(PropertyName);


// Struct to encapsulate effect-related properties
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() : SourceASC(nullptr),
	SourceAvatarActor(nullptr), SourceController(nullptr), SourceCharacter(nullptr), TargetASC(nullptr),
	TargetAvatarActor(nullptr), TargetController(nullptr), TargetCharacter(nullptr)
	{}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY() UAbilitySystemComponent* SourceASC;

	UPROPERTY() AActor* SourceAvatarActor;

	UPROPERTY() AController* SourceController;

	UPROPERTY() ACharacter* SourceCharacter;

	UPROPERTY() UAbilitySystemComponent* TargetASC;

	UPROPERTY() AActor* TargetAvatarActor;

	UPROPERTY() AController* TargetController;

	UPROPERTY() ACharacter* TargetCharacter;
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Map to store gameplay tags and their respective attributes
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	// Primary Attributes
	ATTRIBUTE_PROPERTY(Strength);
	ATTRIBUTE_PROPERTY(Intelligence);
	ATTRIBUTE_PROPERTY(Resilience);
	ATTRIBUTE_PROPERTY(Vigor);

	// Secondary Attributes
	ATTRIBUTE_PROPERTY(Armor);
	ATTRIBUTE_PROPERTY(ArmorPenetration);
	ATTRIBUTE_PROPERTY(BlockChance);
	ATTRIBUTE_PROPERTY(CriticalHitChance);
	ATTRIBUTE_PROPERTY(CriticalHitDamage);
	ATTRIBUTE_PROPERTY(CriticalHitResistance);
	ATTRIBUTE_PROPERTY(HealthRegeneration);
	ATTRIBUTE_PROPERTY(ManaRegeneration);

	// Vital Attributes
	ATTRIBUTE_PROPERTY(Health);
	ATTRIBUTE_PROPERTY(Mana);
	ATTRIBUTE_PROPERTY(MaxHealth);
	ATTRIBUTE_PROPERTY(MaxMana);

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
