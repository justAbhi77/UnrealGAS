//


#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * Custom Asset Manager class for the Aura project.
 * It ensures proper initialization of game assets and handles native gameplay tag setup.
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	 // Called during the initial loading phase of the engine.
	virtual void StartInitialLoading() override;
};
