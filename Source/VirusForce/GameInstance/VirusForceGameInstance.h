// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../SaveGame/VirusForceSaveGame.h" 
#include "VirusForceGameInstance.generated.h"

UCLASS()
class VIRUSFORCE_API UVirusForceGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UVirusForceGameInstance();
protected:
	virtual void Init() override;

private:
	TSubclassOf<class UHighScoreWidget> HighScoreWidgetClass;
	
	TArray<FHighScoreStruct> HighScoreList;

	class UVirusForceSaveGame* SavedGame;

	class UHighScoreWidget* HighScoreWidget;

	int32 FinalScore;

	void LoadHighScores();

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	class UVirusForceSaveGame* LoadedGame;

	void CreateHighScoreList();

	void DisplayHighScoreScreen(int32 Score);

	void SaveHighScore(FText Name);
};
