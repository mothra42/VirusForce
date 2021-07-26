// Copyright Adam Farmer 2020


#include "Arena.h"
#include "../Player/VirusForcePawn.h"
#include "Components/SceneComponent.h"
#include "../NPC/Virus.h"
#include "../NPC/Viruses/StraightVirus.h"
#include "../NPC/Viruses/TrackingVirus.h"
#include "../NPC/Viruses/BurstVirus.h"
#include "../NPC/NeutralCells/InfectableCell.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Public/EngineUtils.h"
#include "../GameMode/VirusForceGameMode.h"
#include "TimerManager.h"
#include "Containers/Queue.h"


// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(-2950, -2950, 0);
	MaxExtent = FVector(2950, 2950, 0);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();

	//Set Arena in game mode
	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		GameMode->SetArena(this);
	}
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ConsumeSpawnQueue();
}


//called externally from wave manager
void AArena::SpawnVirus(TSubclassOf<AVirus> VirusClass)
{
	FVector SpawnLocation;
	float VirusMeshRadius = VirusClass->GetDefaultObject<AVirus>()->MeshRadius;
	if (FindEmptyLocation(SpawnLocation, VirusMeshRadius))
	{
		PlaceVirus(FVector(SpawnLocation.X, SpawnLocation.Y, 0.f), VirusClass);
	}
}

//helper function
bool AArena::bCanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation + FVector(1, 1, 1), //small offset so sweep will work
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(Radius)
		);

	return !HasHit;
}

//helper function
bool AArena::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);
	const int32 MAX_ATTEMPTS = 25;

	for (int32 i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandLocation = FMath::RandPointInBox(Bounds);
		if (bCanSpawnAtLocation(RandLocation, Radius))
		{
			OutLocation = RandLocation;
			return true;
		}
	}
	return false;
}

//helper function
void AArena::PlaceVirus(FVector SpawnPoint, TSubclassOf<AVirus> VirusClass)
{
	UWorld* World = GetWorld();
	if (World != NULL && VirusClass != nullptr)
	{
		FRotator RandRotator = UKismetMathLibrary::RandomRotator();
		AVirus* SpawnedVirus = World->SpawnActor<AVirus>(VirusClass, FVector(SpawnPoint.X, SpawnPoint.Y, 0.f), FRotator(0.f, RandRotator.Yaw, 0.f));
	}
}

//called externally from wave manager
void AArena::PopulateSpawnQueue(TSubclassOf<AVirus> VirusClass, int32 Iterations)
{
	for (int32 i = 0; i < Iterations; i++)
	{
		for (int32 j = 0; j < SpawnPointLocations.Num(); j++)
		{
			FRotator RandRotator = UKismetMathLibrary::RandomRotator();
			FSpawnInstructions SpawnInstructions(SpawnPointLocations[j], VirusClass, FRotator(0.f, RandRotator.Yaw, 0.f));
			SpawnQueue.Enqueue(SpawnInstructions);
		}
	}
}

void AArena::ClearSpawnQueue()
{
	SpawnQueue.Empty();
}

void AArena::ConsumeSpawnQueue()
{
	if (!SpawnQueue.IsEmpty() && GetWorldTimerManager().GetTimerRemaining(TimerHandle_SpawnDelayTimer) <= 0)
	{
		FSpawnInstructions SpawnInstructions;
		SpawnQueue.Dequeue(SpawnInstructions);

		SpawnMassWaveDel.BindUFunction(this, FName("SpawnVirusForMassWave"), SpawnInstructions);
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnDelayTimer, SpawnMassWaveDel, SpawnDelayTime, false);
	}
}

//helper function
void AArena::SpawnVirusForMassWave(FSpawnInstructions SpawnInstructions)
{
	UWorld* World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<AVirus>(SpawnInstructions.VirusClassToSpawn, SpawnInstructions.Location, SpawnInstructions.Rotation);
	}
}