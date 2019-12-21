// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseWaypoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "TestingGrounds/TP_ThirdPerson/PatrollingGuard.h"

EBTNodeResult::Type UChooseWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//TODO protect against empty patrol routes

	//Get the patrol points
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	APatrollingGuard* PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	TArray<AActor*> PatrolPoints = PatrollingGuard->PatrolPoints;

	//Set next waypoint
	UBlackboardComponent* BlackboadComponent = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboadComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboadComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	//Cycle the index
	int32 NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboadComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
	return EBTNodeResult::Succeeded;
}