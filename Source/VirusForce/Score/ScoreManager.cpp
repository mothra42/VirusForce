// Copyright Adam Farmer 2020


#include "ScoreManager.h"
#include "../NPC/Virus.h"

// Sets default values for this component's properties
UScoreManager::UScoreManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScoreManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UScoreManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UScoreManager::IncreaseScore(AVirus* Virus)
{
	return 0;
}

int32 UScoreManager::FindScoreByVirusType(EVirusType VirusType)
{
	switch(VirusType)
	{
	case EVirusType::BaseVirus:
		return BaseVirusScore;
	case EVirusType::StraightVirus:
		return StraightVirusScore;
	case EVirusType::TrackingVirus:
		return TrackingVirusScore;
	default: 
		return 0;
	}
}

int32 UScoreManager::IncreaseVirusConsumedCount()
{
	return NumberOfVirusConsumed++;
}

int32 UScoreManager::SetScoreMultiplier()
{
	return 0;
}