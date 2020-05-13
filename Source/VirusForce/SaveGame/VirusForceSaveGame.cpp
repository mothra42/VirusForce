// Copyright Adam Farmer 2020


#include "VirusForceSaveGame.h"
#include "Kismet/GameplayStatics.h"

UVirusForceSaveGame::UVirusForceSaveGame()
{
	SaveSlotName = TEXT("HighScoreList");
	UserIndex = 0;
}

TArray<FHighScoreStruct> UVirusForceSaveGame::SaveHighScore(FString PlayerName, int32 Score)
{
	FHighScoreStruct HighScore = FHighScoreStruct(PlayerName, Score);

	HighScoreList.Add(HighScore);

	return HighScoreList;
}