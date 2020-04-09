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
	
}

int32 UScoreManager::IncreaseScore(AVirus* Virus, int32 MarkedVirusMultiplier)
{
	int32 BaseScore = FindScoreByVirusType(Virus->VirusType);
	int32 AntibodyMultiplier = GetAntibodyMultiplier(Virus);
	Score += (BaseScore * AntibodyMultiplier * Multiplier * MarkedVirusMultiplier);

	NumberOfVirusConsumed++;

	return Score;
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

//tracks how many viruses have been consumed since death and sets multiplier accordingly
int32 UScoreManager::SetScoreMultiplier()
{
	if (NumberOfVirusConsumed >= ThresholdOne && NumberOfVirusConsumed < ThresholdTwo)
	{
		Multiplier = MultiplierTwo;
	}
	else if (NumberOfVirusConsumed >= ThresholdTwo && NumberOfVirusConsumed < ThresholdThree)
	{
		Multiplier = MultiplierThree;
	}
	else if (NumberOfVirusConsumed >= ThresholdThree && NumberOfVirusConsumed < ThresholdFour)
	{
		Multiplier = MultiplierFour;
	}
	else if (NumberOfVirusConsumed >= ThresholdFour && NumberOfVirusConsumed < ThresholdFive)
	{
		Multiplier = MultiplierFive;
	}
	else if (NumberOfVirusConsumed >= ThresholdFive && NumberOfVirusConsumed < ThresholdSix)
	{
		Multiplier = MultiplierSix;
	}
	else if (NumberOfVirusConsumed >= ThresholdSix && NumberOfVirusConsumed < ThresholdSeven)
	{
		Multiplier = MultiplierSeven;
	}
	else if (NumberOfVirusConsumed >= ThresholdEight && NumberOfVirusConsumed < ThresholdNine)
	{
		Multiplier = MultiplierEight;
	}
	else if (NumberOfVirusConsumed >= ThresholdNine && NumberOfVirusConsumed < ThresholdTen)
	{
		Multiplier = MultiplierNine;
	}
	else if (NumberOfVirusConsumed >= ThresholdTen)
	{
		Multiplier = MultiplierTen;
	}
	else
	{
		Multiplier = MultiplierOne;
	}
	return Multiplier;
}

int32 UScoreManager::GetAntibodyMultiplier(AVirus* Virus)
{
	//TODO fix this right now will return 0 as it is just subtracting
	// the number of attached slots from the max number of slots possible
	// fix this in the virus class
	return Virus->NumOfAttachedAntibodies();
}