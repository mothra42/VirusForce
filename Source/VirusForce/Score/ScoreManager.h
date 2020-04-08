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

public:	
	// Sets default values for this component's properties
	UScoreManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 IncreaseScore(class AVirus* Virus);

	int32 FindScoreByVirusType(EVirusType VirusType);

	int32 IncreaseVirusConsumedCount();

	int32 SetScoreMultiplier();
};
