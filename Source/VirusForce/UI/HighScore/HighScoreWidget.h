// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../SaveGame/VirusForceSaveGame.h" //not ideal but seems to be the only way to get access to struct
#include "HighScoreWidget.generated.h"


struct FHighScoreStruct;
/**
 * 
 */
UCLASS()
class VIRUSFORCE_API UHighScoreWidget: public UUserWidget
{
	GENERATED_BODY()

	UHighScoreWidget(const FObjectInitializer& ObjectInitializer);
public:
	void Setup(TArray<FHighScoreStruct> HighScores);

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HighScoreList;

private:
	TSubclassOf<class UScoreElement> ScoreElementWidgetClass;
	void PopulateHighScores(TArray<FHighScoreStruct> HighScores);

};
