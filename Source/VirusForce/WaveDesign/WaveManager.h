// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaveManager.generated.h"

UENUM()
enum class EWaveType : uint8
{
	BaseVirusWave,
	StraightVirusWave,
	TrackingVirusWave,
	BaseAndStraightMixWave,
	StraightAndTrackingMixWave,
	BaseAndTrackingMixWave,
	TripleVirusWave
};

enum class EVirusType : uint8;

UCLASS( ClassGroup=(Custom), Blueprintable,  meta=(BlueprintSpawnableComponent) )
class VIRUSFORCE_API UWaveManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaveManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Category = Viruses, EditDefaultsOnly)
	TSubclassOf<class AVirus> Virus;
	UPROPERTY(Category = Viruses, EditDefaultsOnly)
	TSubclassOf<AVirus> StraightVirus;
	UPROPERTY(Category = Viruses, EditDefaultsOnly)
	TSubclassOf<AVirus> TrackingVirus;

	TSubclassOf<AVirus> CurrentlySpawningVirusType;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EWaveType DetermineMassWaveSpawnType();

private:

	FTimerHandle TimerHandle_WaveTimerExpired;

	int32 WaveCycle = 0;

	TSubclassOf<AVirus> CycleSpawnedVirusType();
};
