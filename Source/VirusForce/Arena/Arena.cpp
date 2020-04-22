// Copyright Adam Farmer 2020


#include "Arena.h"
#include "Components/SceneComponent.h"
#include "../NPC/Virus.h"
#include "../NPC/Viruses/StraightVirus.h"
#include "../NPC/Viruses/TrackingVirus.h"
#include "../WaveDesign/WaveManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Public/EngineUtils.h"
#include "TimerManager.h"
#include "Containers/Queue.h"


// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(-3000, -3000, 0);
	MaxExtent = FVector(3000, 3000, 0);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();

	//Wave Manger must be set in blueprint
	WaveManager = FindComponentByClass<UWaveManager>();

	GetWorldTimerManager().SetTimer(TimerHandle_MassSpawnTimer, this, &AArena::PopulateSpawnQueue, 30.f, true, 5.f);
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaveManager != nullptr && WaveManager->bNextWaveCanSpawn == true)
	{
		SpawnVirus();
	}

	ConsumeSpawnQueue();
}

void AArena::SpawnVirus()
{
	FVector SpawnLocation;

	if (WaveManager != nullptr)
	{
		float VirusMeshRadius = WaveManager->CurrentlySpawningVirusType->GetDefaultObject<AVirus>()->MeshRadius;
		if (FindEmptyLocation(SpawnLocation, VirusMeshRadius))
		{
			PlaceVirus(FVector(SpawnLocation.X, SpawnLocation.Y, 0.f), WaveManager->CurrentlySpawningVirusType);
		}
	}
}

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
		FCollisionShape::MakeSphere(50.f)
		);

	return !HasHit;
}

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

void AArena::PlaceVirus(FVector SpawnPoint, TSubclassOf<AVirus> VirusClass)
{
	UWorld* World = GetWorld();
	if (World != NULL && VirusClass != nullptr)
	{
		FRotator RandRotator = UKismetMathLibrary::RandomRotator();
		AVirus* SpawnedVirus = World->SpawnActor<AVirus>(VirusClass, FVector(SpawnPoint.X, SpawnPoint.Y, 0.f), FRotator(0.f, RandRotator.Yaw, 0.f));
	}
}

void AArena::PopulateSpawnQueue()
{
	UE_LOG(LogTemp, Warning, TEXT("Populating queue"));
	for (int32 i = 0; i < MassSpawnIterations; i++)
	{
		for (int32 j = 0; j < SpawnPointLocations.Num(); j++)
		{
			FRotator RandRotator = UKismetMathLibrary::RandomRotator();
			FSpawnInstructions SpawnInstructions(SpawnPointLocations[j], WaveManager->Virus, FRotator(0.f, RandRotator.Yaw, 0.f));
			SpawnQueue.Enqueue(SpawnInstructions);
		}
	}
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

void AArena::SpawnVirusForMassWave(FSpawnInstructions SpawnInstructions)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Locations would be %s"), *SpawnInstructions.Location.ToString());
	UWorld* World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<AVirus>(SpawnInstructions.VirusClassToSpawn, SpawnInstructions.Location, SpawnInstructions.Rotation);
	}
}