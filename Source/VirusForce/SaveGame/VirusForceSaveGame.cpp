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

//void UVirusForceSaveGame::LoadGameDelegate(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
//{
//	UVirusForceSaveGame* SaveGame = Cast<UVirusForceSaveGame>(LoadedGameData);
//
//	UE_LOG(LogTemp, Warning, TEXT("loaded game data, high score list is %i long"), SaveGame->HighScoreList.Num());
//}