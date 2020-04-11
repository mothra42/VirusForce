// Copyright Adam Farmer 2020


#include "ScoreManager.h"
#include "../NPC/Virus.h"
#include "../NPC/Viruses/StraightVirus.h"
#include "../NPC/Viruses/TrackingVirus.h"

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
	
}

int32 UScoreManager::IncreaseScore(AVirus* Virus, int32 MarkedVirusMultiplier)
{
	int32 BaseScore = FindScoreByVirusType(FindVirusType(Virus));
	int32 AntibodyMultiplier = GetAntibodyMultiplier(Virus);
	Score += (BaseScore * AntibodyMultiplier * MarkedVirusMultiplier);

	NumberOfVirusConsumed++;
	return Score;
}

//helper function to determine what type of virus was consumed
//as more virus types are added to the game they will need to be added here
EVirusType UScoreManager::FindVirusType(AVirus* Virus)
{
	AStraightVirus* StraightVirus = Cast<AStraightVirus>(Virus);
	ATrackingVirus* TrackingVirus = Cast<ATrackingVirus>(Virus);

	if (StraightVirus != nullptr)
	{
		return StraightVirus->VirusType;
	}
	else if (TrackingVirus != nullptr)
	{
		return TrackingVirus->VirusType;
	}
	else
	{
		return Virus->VirusType;
	}
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

int32 UScoreManager::GetAntibodyMultiplier(AVirus* Virus)
{
	return Virus->NumOfAttachedAntibodies();
}