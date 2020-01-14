// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Runtime/Engine/Public/WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

#include "ActorPool.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::SetPool(class UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));

	Pool = InPool;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	//CanSpawnAtLocation(GetActorLocation(), 300);

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	FTimerHandle SpawnMeshesTimerHandle;
	FTimerDelegate SpawnMeshesTimerDelegate;

	//TODO write simple timer like BP Delay
	//reduce hitches
	SpawnMeshesTimerDelegate.BindUFunction(this, FName("DelayedPlaceActors"), ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
	GetWorldTimerManager().SetTimer(SpawnMeshesTimerHandle, SpawnMeshesTimerDelegate, .2f, false);
	//DelayedPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::DelayedPlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	//FTimerHandle SpawnMeshesTimerHandle[100];
	//FTimerDelegate SpawnMeshesTimerDelegate[100];

	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn); 
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(SpawnPoint, Radius * RandomScale))
		{
			float RandomRotation = FMath::RandRange(-180.f, 180.f);

			//reduce hitches
			FTimerHandle SpawnMeshesTimerHandle;
			FTimerDelegate SpawnMeshesTimerDelegate;
			SpawnMeshesTimerDelegate.BindUFunction(this, FName("PlaceActor"), ToSpawn, SpawnPoint, RandomRotation, RandomScale);
			GetWorldTimerManager().SetTimer(SpawnMeshesTimerHandle, SpawnMeshesTimerDelegate, .01f * static_cast<float>(i), false);

			//PlaceActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale);
		}	

		//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *SpawnPoint.ToCompactString());
	}
}


bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FVector Min(0, -2000, 200);//z -1000 / -1100
	FVector Max(2500, 2000, 200);// x 0-4000
	FBox Bounds(Min, Max);
	//FBox Bounds(MinLocation, MaxLocation);
	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale)
{
	AActor* Spawned = nullptr;
	if(ensure(GetWorld()))
		Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, Rotation, 0));
		Spawned->SetActorScale3D(FVector(Scale));
	}
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FColor ResultColor;
	bool HasHit;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	if (ensure(GetWorld()))
	{
		HasHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			GlobalLocation,
			GlobalLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(Radius)
		);
		ResultColor = HasHit ? FColor::Red : FColor::Green;
		//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
		return !HasHit;
	}
	
	return false;
}

