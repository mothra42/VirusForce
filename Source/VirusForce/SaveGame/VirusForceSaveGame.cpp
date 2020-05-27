// Copyright Adam Farmer 2020


#include "VirusForceSaveGame.h"
#include "Kismet/GameplayStatics.h"

UVirusForceSaveGame::UVirusForceSaveGame()
{
	SaveSlotName = TEXT("TestSlot");
	UserIndex = 0;
}

TArray<FHighScoreStruct> UVirusForceSaveGame::SaveHighScore(FString PlayerName, int32 Score)
{
	HighScoreElement = FHighScoreStruct(PlayerName, Score);
	MyScore = Score;
	HighScoreList.Add(HighScoreElement);
	ScoreArray.Add(Score);
	UE_LOG(LogTemp, Warning, TEXT("printing out struct after adding to array"));
	//PrintOutInfo();
	return HighScoreList;
}

void UVirusForceSaveGame::PrintOutInfo()
{
	for (size_t i = 0; i < HighScoreList.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Name is %s, Score is %i, MyScore is %i, Score stored in array %i"), *HighScoreList[i].PlayerName, HighScoreList[i].Score, MyScore, ScoreArray[i]);
	}
}