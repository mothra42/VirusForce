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
	
	int32 NumberOfVirusConsumed = 0;

	//Scores for viruses
	int32 BaseVirusScore = 100;
	int32 StraightVirusScore = 300;
	int32 TrackingVirusScore = 500;

public:	
	// Sets default values for this component's properties
	UScoreManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(Category = Score, BlueprintReadOnly)
	int32 Score = 0;

	//increases the score
	int32 IncreaseScore(class AVirus* Virus, int32 MarkedVirusMultiplier);

private:

	//finds virus type of consumed virus
	EVirusType FindVirusType(AVirus* Virus);

	//finds score for virus type
	int32 FindScoreByVirusType(EVirusType VirusType);

	//tracks viruses consumed
	int32 IncreaseVirusConsumedCount();

	//if the virus is fully marked give a bonus multiplier
	int32 GetAntibodyMultiplier(AVirus* Virus);
};
