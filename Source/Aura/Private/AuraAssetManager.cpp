// 


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	// Cast and return the AssetManager as UAuraAssetManager
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Initialize the native gameplay tags for the project.
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Gameplay Abilities Target Data Structure
	UAbilitySystemGlobals::Get().InitGlobalData();
}
