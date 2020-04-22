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
	
	void SpawnMassWave(TArray<TSubclassOf<class AVirus>> VirusTypesToSpawn);

private:

	TSubclassOf<AVirus> VirusClassToSpawn;

	bool bCanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PlaceVirus(FVector SpawnPoint, TSubclassOf<AVirus> VirusClass);
};
