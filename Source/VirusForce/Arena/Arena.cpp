// Copyright Adam Farmer 2020


#include "Arena.h"
#include "../Player/VirusForcePawn.h"
#include "Components/SceneComponent.h"
#include "../NPC/Virus.h"
#include "../NPC/Viruses/StraightVirus.h"
#include "../NPC/Viruses/TrackingVirus.h"
#include "../NPC/Viruses/BurstVirus.h"
#include "../NPC/NeutralCells/InfectableCell.h"
#include "../WaveDesign/WaveManager.h"
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

	MinExtent = FVector(-3000, -3000, 0);
	MaxExtent = FVector(3000, 3000, 0);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();

	//Wave Manger must be set in blueprint
	WaveManager = FindComponentByClass<UWaveManager>();

	WaveManager->OnInfectableCellThresholdPassed.AddDynamic(this, &AArena::SpawnInfectableCell);
	//Set Arena in game mode
	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		GameMode->SetArena(this);
	}

	GetWorldTimerManager().SetTimer(
		TimerHandle_SpawnSingleVirusTimer,
		this,
		&AArena::SpawnVirus,
		5.f,
		true,
		0.f);

	//timer for calling mass wave functions
	GetWorldTimerManager().SetTimer(
		TimerHandle_MassSpawnTimer, 
		this, 
		&AArena::PopulateSpawnQueue, 
		TimeBetweenMassWaves, 
		true, 
		DelayUntilMassWavesBegin);
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ConsumeSpawnQueue();
}

void AArena::SpawnVirus()
{
	FVector SpawnLocation;
	TSubclassOf<AVirus> VirusClass = WaveManager->CycleSpawnedVirusType();
	if (WaveManager != nullptr)
	{
		for (int32 i = 0; i < 3; i++)
		{
			float VirusMeshRadius = VirusClass->GetDefaultObject<AVirus>()->MeshRadius;
			if (FindEmptyLocation(SpawnLocation, VirusMeshRadius))
			{
				PlaceVirus(FVector(SpawnLocation.X, SpawnLocation.Y, 0.f), VirusClass);
			}
		}
	}
}

void AArena::SpawnInfectableCell()
{
	FVector SpawnLocation;

	//TODO set a radius in infectable cell class
	float MeshRadius = 65.f;
	if (FindEmptyLocation(SpawnLocation, MeshRadius))
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning infectable cell"));
		PlaceInfectableCell(FVector(SpawnLocation.X, SpawnLocation.Y, 0.f), InfectableCellClassToSpawn);
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
		FCollisionShape::MakeSphere(Radius)
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

void AArena::PlaceInfectableCell(FVector SpawnPoint, TSubclassOf<AInfectableCell> InfectableCell)
{
	UWorld* World = GetWorld();
	if (World != NULL && InfectableCell != nullptr)
	{
		FRotator RandRotator = UKismetMathLibrary::RandomRotator();
		World->SpawnActor<AInfectableCell>(InfectableCell, FVector(SpawnPoint.X, SpawnPoint.Y, 0.f), FRotator(0.f, RandRotator.Yaw, 0.f));
	}
}

void AArena::PopulateSpawnQueue()
{
	EWaveType WaveType = WaveManager->DetermineMassWaveSpawnType();
	for (int32 i = 0; i < MassSpawnIterations; i++)
	{
		for (int32 j = 0; j < SpawnPointLocations.Num(); j++)
		{
			FRotator RandRotator = UKismetMathLibrary::RandomRotator();
			FSpawnInstructions SpawnInstructions(SpawnPointLocations[j], GetVirusTypeToSpawn(WaveType, i), FRotator(0.f, RandRotator.Yaw, 0.f));
			SpawnQueue.Enqueue(SpawnInstructions);
		}
	}
}

//index here represents which corner the virus is being spawned from
TSubclassOf<AVirus> AArena::GetVirusTypeToSpawn(EWaveType WaveType, int32 index)
{
	switch (WaveType)
	{
		case EWaveType::BaseVirusWave:
			return WaveManager->Virus;
		case EWaveType::StraightVirusWave:
			return WaveManager->StraightVirus;
		case EWaveType::TrackingVirusWave:
			return WaveManager->TrackingVirus;
		case EWaveType::BaseAndStraightMixWave:
			if (index == 0 || index == 2)
			{
				return WaveManager->Virus;
			}
			else
			{
				return WaveManager->StraightVirus;
			}
		case EWaveType::BaseAndTrackingMixWave:
			if (index == 0 || index == 2)
			{
				return WaveManager->Virus;
			}
			else
			{
				return WaveManager->TrackingVirus;
			}
		case EWaveType::StraightAndTrackingMixWave:
			if (index == 0 || index == 2)
			{
				return WaveManager->StraightVirus;
			}
			else
			{
				return WaveManager->TrackingVirus;
			}
		case EWaveType::TripleVirusWave:
			if (index == 0 || index == 2)
			{
				return WaveManager->StraightVirus;
			}
			else if (index == 1)
			{
				return WaveManager->TrackingVirus;
			}
			else
			{
				return WaveManager->Virus;
			}
		default:
			return WaveManager->Virus;
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
	UWorld* World = GetWorld();
	if (World != NULL)
	{
		World->SpawnActor<AVirus>(SpawnInstructions.VirusClassToSpawn, SpawnInstructions.Location, SpawnInstructions.Rotation);
	}
}