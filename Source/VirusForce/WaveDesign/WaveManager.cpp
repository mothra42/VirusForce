// Copyright Adam Farmer 2020


#include "WaveManager.h"
#include "../NPC/Virus.h"
#include "../Arena/Arena.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWaveManager::UWaveManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWaveManager::BeginPlay()
{
	Super::BeginPlay();

	Arena = Cast<AArena>(UGameplayStatics::GetActorOfClass(GetWorld(), AArena::StaticClass()));
}


// Called every frame
void UWaveManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//called from blueprint
void UWaveManager::SetupSpawnVirus(TSubclassOf<AVirus> VirusClass)
{
	Arena->SpawnVirus(VirusClass);
}

void UWaveManager::SetupMassSpawn(TSubclassOf<AVirus> VirusClass, int32 Iterations)
{
	Arena->PopulateSpawnQueue(VirusClass, Iterations);
}