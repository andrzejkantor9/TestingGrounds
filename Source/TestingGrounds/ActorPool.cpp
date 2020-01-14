// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UActorPool::Checkout()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checkout."), *GetName());
	return nullptr;
}
void UActorPool::Return(AActor* ActorToReturn)
{
	if (ensure(ActorToReturn))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Actor returned {%s}"), *GetName(), *ActorToReturn->GetName());
	}	
}
void UActorPool::Add(AActor* ActorToAdd)
{
	if (ensure(ActorToAdd))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Actor added {%s}"), *GetName(), *ActorToAdd->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Nullptr actor added {%s}"), *GetName());
	}
}