// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

UCLASS()
class VIRUSFORCE_API AArena : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(Category = WaveManagement, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWaveManager* WaveManager;

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

private:

	TSubclassOf<class AVirus> VirusClassToSpawn;

	bool bCanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PlaceVirus(FVector SpawnPoint, TSubclassOf<AVirus> VirusClass);
};
