// Copyright Adam Farmer 2020


#include "Arena.h"
#include "DrawDebugHelpers.h"
#include "../NPC/Virus.h"

// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(-6000, -6000, 0);
	MaxExtent = FVector(6000, 6000, 0);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();

	FVector Corner1 = FVector(-5800, -5800, 0);
	FVector Corner2 = FVector(5800, -5800, 0);
	FVector Corner3 = FVector(-5800, 5800, 0);
	FVector Corner4 = FVector(5800, 5800, 0);

	FBox Bounds(MinExtent, MaxExtent);
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		//TODO use the FBOX to spawn viruses at timed intervals.
	}
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArena::SpawnVirus()
{

}

bool AArena::bCanSpawnAtLocation(FVector Location, float Radius)
{
	return false;
}

bool AArena::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	return false;
}