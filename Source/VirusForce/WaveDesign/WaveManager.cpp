// Copyright Adam Farmer 2020


#include "WaveManager.h"
#include "../NPC/Virus.h"

// Sets default values for this component's properties
UWaveManager::UWaveManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWaveManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UWaveManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* const World = GetWorld();
}

//Method to set the spawned virus type
TSubclassOf<AVirus> UWaveManager::CycleSpawnedVirusType()
{
	//UE_LOG(LogTemp, Warning, TEXT("I'm on wave %i"), WaveCycle);
	if (Virus != nullptr && StraightVirus != nullptr && StraightVirus != nullptr)
	{
		//every 10 cycles spawn a new infectable cell in the arena.
		if (WaveCycle % 10 == 0)
		{
			OnInfectableCellThresholdPassed.Broadcast();
		}

		if (WaveCycle % 7 == 0)
		{
			WaveCycle++;
			return TrackingVirus;
		}
		else if (WaveCycle % 5 == 0)
		{
			WaveCycle++;
			return StraightVirus;
		}
		else if (WaveCycle % 11 == 0)
		{
			WaveCycle++;
			return BurstVirus;
		}
		else 
		{ 
			WaveCycle++;
			return Virus; 
		}
	}
	return nullptr;
}

EWaveType UWaveManager::DetermineMassWaveSpawnType()
{
	int32 RandInt = FMath::RandRange(0, 6);

	switch(RandInt)
	{
		case 0:
			return EWaveType::BaseVirusWave;
		case 1:
			return EWaveType::StraightVirusWave;
		case 2:
			return EWaveType::TrackingVirusWave;
		case 3:
			return EWaveType::BaseAndStraightMixWave;
		case 4:
			return EWaveType::BaseAndTrackingMixWave;
		case 5:
			return EWaveType::StraightAndTrackingMixWave;
		case 6:
			return EWaveType::TripleVirusWave;
		default:
			return EWaveType::BaseVirusWave;
	}
}