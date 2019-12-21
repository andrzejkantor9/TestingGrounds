// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseWaypoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "TestingGrounds/ActorComponents/PatrolRoute.h"

EBTNodeResult::Type UChooseWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get the patrol route
	//AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	UPatrolRoute* PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	//APatrollingGuard* PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	TArray<AActor*> PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points."));
		return EBTNodeResult::Failed;
	}

	//Set next waypoint
	UBlackboardComponent* BlackboadComponent = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboadComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboadComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	//Cycle the index
	int32 NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboadComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}