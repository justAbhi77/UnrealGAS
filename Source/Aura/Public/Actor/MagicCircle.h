// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:
	AMagicCircle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
};
