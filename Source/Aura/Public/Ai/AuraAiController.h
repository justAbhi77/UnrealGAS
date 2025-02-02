// 

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AuraAiController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class AURA_API AAuraAiController : public AAIController
{
	GENERATED_BODY()

public:
	AAuraAiController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;

public:
	virtual void Tick(float DeltaTime) override;
};
