// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WaveManager.generated.h"


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
	UPROPERTY(Category = WaveTiming, EditDefaultsOnly)
	float EnemyWaveRate;

	bool bNextWaveCanSpawn = false;

	UPROPERTY(Category = Viruses, EditDefaultsOnly)
	TSubclassOf<class AVirus> Virus;

	TSubclassOf<AVirus> CurrentlySpawningVirusType;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FTimerHandle TimerHandle_WaveTimerExpired;

	int32 WaveCycle = 1;
	
	void SetWaveCanSpawn();

	TSubclassOf<AVirus> CycleSpawnedVirusType();
};
