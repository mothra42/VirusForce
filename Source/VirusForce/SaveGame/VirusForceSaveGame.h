// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VirusForceSaveGame.generated.h"

USTRUCT()
struct FHighScoreStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
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
	//UPROPERTY(VisibleAnywhere, Category = Basic)
	//FHighScoreStruct HighScoreElement;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FHighScoreStruct> HighScoreList;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 UserSaveIndex;

	TArray<FHighScoreStruct> SaveHighScore(FString PlayerName, int32 Score);

	TArray<FHighScoreStruct> LoadSavedGame();

	//DEV ONLY
	void PrintOutInfo();

private:
	void DelegateAsyncSave();

	void SaveGameDelegate(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
