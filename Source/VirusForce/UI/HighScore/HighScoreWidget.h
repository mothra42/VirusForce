// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	TSubclassOf<class UNameInputWidget> NameInputWidgetClass;

	void PopulateHighScores(TArray<FHighScoreStruct> HighScores);

};
