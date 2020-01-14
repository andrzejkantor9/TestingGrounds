// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Runtime/Engine/Public/WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "NavMesh/NavMeshBoundsVolume.h"

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
 
	if (ensure(Pool))
	{
		PositionNavMeshBoundsVolume();
	}
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (ensure(NavMeshBoundsVolume))
	{
		NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
		UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
		if (ensure(GetWorld() && GetWorld()->GetNavigationSystem()))
		{
			//GetWorld()->GetNavigationSystem()->Build();
			FNavigationSystem::Build(*GetWorld());
		}
	}	
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough NavMeshes in pool."), *GetName());
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	//CanSpawnAtLocation(GetActorLocation(), 300);

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ensure(Pool))
	{
		Pool->Return(NavMeshBoundsVolume);
	}
	//UE_LOG(LogTemp, Warning, TEXT("[%s] EndPlay"), *GetName());
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, 1, 1);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceAIPawn(ToSpawn, SpawnPosition);
	}
}

void ATile::PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	APawn* Spawned = nullptr;
	if (ensure(GetWorld()))
		Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));

		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
		//TODO change tag to AI
		//Spawned->Tags.Add(FName("AI"));
	}
}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	FTimerHandle SpawnMeshesTimerHandle;
	FTimerDelegate SpawnMeshesTimerDelegate;
	
	//TODO write simple timer like BP Delay
	//reduce hitches
	SpawnMeshesTimerDelegate.BindUFunction(this, FName("DelayedPlaceActors"), ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
	GetWorldTimerManager().SetTimer(SpawnMeshesTimerHandle, SpawnMeshesTimerDelegate, .2f * TimesCalled, false);
	TimesCalled += 1.f;
	//DelayedPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::DelayedPlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

TArray<FSpawnPosition> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions;
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			//reduce hitches
			FTimerHandle SpawnMeshesTimerHandle;
			FTimerDelegate SpawnMeshesTimerDelegate;

			//SpawnMeshesTimerDelegate.BindUFunction(this, FName("PlaceActor"), ToSpawn, SpawnPosition.Location, SpawnPosition.Rotation, SpawnPosition.Scale);
			//GetWorldTimerManager().SetTimer(SpawnMeshesTimerHandle, SpawnMeshesTimerDelegate, .002f * static_cast<float>(i), false);
			SpawnPositions.Add(SpawnPosition);
			//PlaceActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale);
		}
	}

	return SpawnPositions;
}


bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinSpawnLocation, MaxSpawnLocation);
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

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	AActor* Spawned = nullptr;
	if(ensure(GetWorld()))
		Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	if (Spawned)
	{
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
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

