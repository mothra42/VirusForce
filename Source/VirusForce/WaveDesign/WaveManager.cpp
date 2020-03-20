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

	CurrentlySpawningVirusType = Virus;
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
		WaveCycle++;

		//set a new virus type to be spawned
		CurrentlySpawningVirusType = CycleSpawnedVirusType();
		World->GetTimerManager().SetTimer(TimerHandle_WaveTimerExpired, this, &UWaveManager::SetWaveCanSpawn, EnemyWaveRate);
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

	return Virus;
}