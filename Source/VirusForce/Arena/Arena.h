// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

enum class EWaveType : uint8;

USTRUCT()
struct FSpawnInstructions
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	TSubclassOf<class AVirus> VirusClassToSpawn;
	FRotator Rotation;

	//This might be bad
	FSpawnInstructions() 
		: Location(FVector()), VirusClassToSpawn(nullptr), Rotation(FRotator()) {}

	FSpawnInstructions(const FVector InLocation, const TSubclassOf<AVirus>& InVirusClassToSpawn, const FRotator InRotator)
		: Location(InLocation), VirusClassToSpawn(InVirusClassToSpawn), Rotation(InRotator) {}
};

UCLASS()
class VIRUSFORCE_API AArena : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(Category = WaveManagement, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWaveManager* WaveManager;

	//set at beginplay in arena blueprint
	UPROPERTY(Category = SpawnLocations, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> SpawnPointLocations;

public:	
	// Sets default values for this actor's properties
	AArena();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Spawning, EditDefaultsOnly)
	FVector MinExtent;
	UPROPERTY(Category = Spawning, EditDefaultsOnly)
	FVector MaxExtent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnVirus();
	
	void PopulateSpawnQueue();

	FORCEINLINE FTimerHandle GetMassSpawnTimer() { return TimerHandle_MassSpawnTimer; }
	FORCEINLINE FTimerHandle GetSpawnTimer() { return TimerHandle_SpawnSingleVirusTimer; }

private:
	//timers for handling mass spawn waves
	FTimerHandle TimerHandle_MassSpawnTimer;
	FTimerHandle TimerHandle_SpawnDelayTimer;
	FTimerDelegate SpawnMassWaveDel;

	//timers for handling constant spawn waves
	FTimerHandle TimerHandle_SpawnSingleVirusTimer;

	UPROPERTY(Category = Spawning, EditDefaultsOnly)
	float DelayUntilMassWavesBegin = 60.f;

	UPROPERTY(Category = Spawning, EditDefaultsOnly)
	float TimeBetweenMassWaves = 30.f;

	//Controls the spacing of viruses spawning in a mass wave
	UPROPERTY(Category = Spawning, EditDefaultsOnly)
	float SpawnDelayTime = 0.3f;

	//used to determine how many viruses are spawned during a mass wave
	//iteration starts at 0 so it is i + 1 to determine iterations
	//also multiply by 4 to get total number of viruses spawning
	UPROPERTY(Category = Spawning, EditDefaultsOnly)
	int32 MassSpawnIterations = 14;

	TSubclassOf<AVirus> VirusClassToSpawn;
	
	TQueue<FSpawnInstructions> SpawnQueue;

	FVector CenterPoint = FVector(0, 0, 0);

	bool bCanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PlaceVirus(FVector SpawnPoint, TSubclassOf<AVirus> VirusClass);

	void ConsumeSpawnQueue();

	UFUNCTION()
	void SpawnVirusForMassWave(FSpawnInstructions SpawnInstructions);

	TSubclassOf<AVirus> GetVirusTypeToSpawn(EWaveType WaveType, int32 index);
};
