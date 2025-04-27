//


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAiController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * AI Controller managing blackboard and behavior tree components.
 */
UCLASS()
class AURA_API AAuraAiController : public AAIController
{
	GENERATED_BODY()

public:
	AAuraAiController();

protected:
	// Behavior tree component handling AI logic
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
};
