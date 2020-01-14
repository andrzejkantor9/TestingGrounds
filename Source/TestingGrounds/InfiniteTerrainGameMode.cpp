// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteTerrainGameMode.h"

#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto NavMeshVolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	//TActorIterator<ANavMeshBoundsVolume> NavMeshVolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (NavMeshVolumeIterator) //if there is any actor left to iterate
	{
		AddToPool(*NavMeshVolumeIterator);
		++NavMeshVolumeIterator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume *VolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Found NavMesh: %s"), *VolumeToAdd->GetName());
}