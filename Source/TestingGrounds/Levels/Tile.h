// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		FVector MinLocation = FVector(-300.000000, -2200.000000, -1100.000000);
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		FVector MaxLocation = FVector(3000.000000, 750.000000, -1100.000000);
public:	
	// Sets default values for this actor's properties
	ATile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Levels")
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn);
};
