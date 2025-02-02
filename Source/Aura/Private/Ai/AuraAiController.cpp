// 


#include "Ai/AuraAiController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AAuraAiController::AAuraAiController()
{
	PrimaryActorTick.bCanEverTick = true;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void AAuraAiController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

