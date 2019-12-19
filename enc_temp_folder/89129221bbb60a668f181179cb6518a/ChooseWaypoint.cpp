// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseWaypoint.h"

EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("DONKEY"));

	return EBTNodeResult::Succeeded;
}