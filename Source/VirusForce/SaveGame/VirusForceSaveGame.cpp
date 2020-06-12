// Copyright Adam Farmer 2020


#include "VirusForceSaveGame.h"
#include "Kismet/GameplayStatics.h"

UVirusForceSaveGame::UVirusForceSaveGame()
{
	SaveSlotName = TEXT("SaveSlot");
	UserSaveIndex = 0;
}

//initialize the saved game object when game loads
TArray<FHighScoreStruct> UVirusForceSaveGame::LoadSavedGame()
{
	if (UVirusForceSaveGame* LoadedGame = Cast<UVirusForceSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserSaveIndex)))
	{
		return HighScoreList = LoadedGame->HighScoreList;
	}

	return HighScoreList;
}

TArray<FHighScoreStruct> UVirusForceSaveGame::SaveHighScore(FString PlayerName, int32 Score)
{
	FHighScoreStruct HighScoreElement = FHighScoreStruct(PlayerName, Score);
	HighScoreList.Add(HighScoreElement);
	PrintOutInfo();
	DelegateAsyncSave();
	return HighScoreList;
}

void UVirusForceSaveGame::PrintOutInfo()
{
	if (HighScoreList.Num() >= 0)
	{
		for (int32 i = 0; i < HighScoreList.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Name is %s, Score is %i"), *HighScoreList[i].PlayerName, HighScoreList[i].Score);
		}
	}
}

void UVirusForceSaveGame::DelegateAsyncSave()
{
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &UVirusForceSaveGame::SaveGameDelegate);
	UGameplayStatics::AsyncSaveGameToSlot(this, SaveSlotName, UserSaveIndex, SavedDelegate);
}

void UVirusForceSaveGame::SaveGameDelegate(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Successful from save game class!"));
	}
}