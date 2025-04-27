//


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
#include "Aura/AuraLogChannels.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UE_LOG(LogAura, Display, TEXT("Initializing Asset Manager from [%hs]"), __FUNCTION__);

	// Cast and return the AssetManager as UAuraAssetManager
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(LogAura, Display, TEXT("Initializing Native GameplayTags from from [%hs]"), __FUNCTION__);
	// Initialize the native gameplay tags for the project.
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Gameplay Abilities Target Data Structure
	UAbilitySystemGlobals::Get().InitGlobalData();
}
