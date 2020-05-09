// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VirusForceSaveGame.generated.h"

USTRUCT()
struct FHighScoreStruct
{
	GENERATED_USTRUCT_BODY()

	FString PlayerName;
	int32 Score;

	FHighScoreStruct() : PlayerName(FString("")), Score(0) {}
	FHighScoreStruct(const FString InPlayerName, const int32 InScore) : PlayerName(InPlayerName), Score(InScore) {}
};

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API UVirusForceSaveGame : public USaveGame
{
	GENERATED_BODY()

	UVirusForceSaveGame();
	
public:
	TArray<FHighScoreStruct> HighScoreList;

	UPROPERTY(VisibleAnywhere, Category = "Basics")
	FString SaveSlotName;
};
