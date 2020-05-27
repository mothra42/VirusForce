// Copyright Adam Farmer 2020


#include "SaveGameHelper.h"
#include "Kismet/GameplayStatics.h"
#include "VirusForceSaveGame.h"

void USaveGameHelper::LoadGameDelegate(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	if (LoadedGameData != nullptr)
	{
		UVirusForceSaveGame* SaveGame = Cast<UVirusForceSaveGame>(LoadedGameData);
		SaveGame->PrintOutInfo();
		UE_LOG(LogTemp, Warning, TEXT("loaded game data, high score list is %i long"), SaveGame->HighScoreList.Num());
	}
}

void USaveGameHelper::SaveGameDelegate(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Successful!"));
		UVirusForceSaveGame* LoadedGame = Cast<UVirusForceSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));
		LoadedGame->PrintOutInfo();
	}
}