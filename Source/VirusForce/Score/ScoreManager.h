// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreManager.generated.h"

enum class EVirusType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRUSFORCE_API UScoreManager : public UActorComponent
{
	GENERATED_BODY()
	
	int32 Score = 0;
	int32 Multiplier = 1;
	int32 NumberOfVirusConsumed = 0;

	//Scores for viruses
	int32 BaseVirusScore = 100;
	int32 StraightVirusScore = 300;
	int32 TrackingVirusScore = 500;

	//Score Multipliers
	int32 MultiplierOne = 1;
	int32 MultiplierTwo = 2;
	int32 MultiplierThree = 3;
	int32 MultiplierFour = 5;
	int32 MultiplierFive = 10;
	int32 MultiplierSix = 50;
	int32 MultiplierSeven = 100;
	int32 MultiplierEight = 300;
	int32 MultiplierNine = 500;
	int32 MultiplierTen = 1000;

	//Multiplier Thresholds
	int32 ThresholdOne = 15;
	int32 ThresholdTwo = 30;
	int32 ThresholdThree = 60;
	int32 ThresholdFour = 100;
	int32 ThresholdFive = 150;
	int32 ThresholdSix = 200;
	int32 ThresholdSeven = 250;
	int32 ThresholdEight = 300;
	int32 ThresholdNine = 350;
	int32 ThresholdTen = 400;

public:	
	// Sets default values for this component's properties
	UScoreManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//increases the score
	int32 IncreaseScore(class AVirus* Virus, int32 MarkedVirusMultiplier);

private:

	//finds score for virus type
	int32 FindScoreByVirusType(EVirusType VirusType);

	//tracks viruses consumed
	int32 IncreaseVirusConsumedCount();

	//sets overall score multiplier based on number of viruses consumed in one life
	int32 SetScoreMultiplier();

	//if the virus is fully marked give a bonus multiplier
	int32 GetAntibodyMultiplier(AVirus* Virus);
};
