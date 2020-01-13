// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Runtime/Engine/Public/WorldCollision.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CastSphere(GetActorLocation(), 300);
	CastSphere(GetActorLocation()+ FVector(0, 0, 1000), 300);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn)
{
	//FVector Min(0, -2000, -1000);
	//FVector Max(4000, 2000, -1000); 
	FBox Bounds(MinLocation, MaxLocation);
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn); 
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		AActor* Spawned = nullptr;
		if(ensure(GetWorld()))
			Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		if (Spawned)
		{
			Spawned->SetActorRelativeLocation(SpawnPoint);
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		}
		//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *SpawnPoint.ToCompactString());
	}
}

bool ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;
	FColor ResultColor;
	bool HasHit;
	if (ensure(GetWorld()))
	{
		HasHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			Location,
			Location,
			FQuat::Identity,
			ECollisionChannel::ECC_Camera,
			FCollisionShape::MakeSphere(Radius)
		);
		ResultColor = HasHit ? FColor::Red : FColor::Green;
		DrawDebugSphere(GetWorld(), Location, Radius, 10, ResultColor, true, 100);
	}
	
	return false;
}

