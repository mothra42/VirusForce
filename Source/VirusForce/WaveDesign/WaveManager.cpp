// Copyright Adam Farmer 2020


#include "WaveManager.h"
#include "../NPC/Virus.h"

// Sets default values for this component's properties
UWaveManager::UWaveManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	EnemyWaveRate = 60.0f;
}


// Called when the game starts
void UWaveManager::BeginPlay()
{
	Super::BeginPlay();

	bNextWaveCanSpawn = true;
	if (Virus != nullptr)
	{
		CurrentlySpawningVirusType = Virus;
	}
}


// Called every frame
void UWaveManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* const World = GetWorld();
	if (World != NULL && bNextWaveCanSpawn == true)
	{
		bNextWaveCanSpawn = false;
		//will be used to determine what wave player is on and what should be spawned

		//set a new virus type to be spawned
		CurrentlySpawningVirusType = CycleSpawnedVirusType();
		World->GetTimerManager().SetTimer(TimerHandle_WaveTimerExpired, this, &UWaveManager::SetWaveCanSpawn, EnemyWaveRate);

		WaveCycle++;
	}
}

void UWaveManager::SetWaveCanSpawn()
{
	bNextWaveCanSpawn = true;
}

//Method to set the spawned virus type, not sure how to handle spawning two different virus types at the same time
//but that would be handled here as well
TSubclassOf<AVirus> UWaveManager::CycleSpawnedVirusType()
{
	//TODO as more virus types are added we need to be able to change what is currently being spawned dynamically
	//can use a round counter to determine what is being randomly spawned
	//THOUGHT have a separate type of wave counter that will spawn mobs of enemies like in geometry wars
	if (Virus != nullptr && StraightVirus != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("My Wave Cycle is %i"), WaveCycle);
		if (WaveCycle % 5 == 0)
		{
			return TrackingVirus;
		}
		if (WaveCycle % 2 == 0)
		{
			return StraightVirus;
		}

		else return Virus;
	}
	return nullptr;
}