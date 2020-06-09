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
	void Setup(TArray<FHighScoreStruct> HighScores, int32 NewScore);

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HighScoreList;

	UFUNCTION()
	void SaveOnPlayerNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	TSubclassOf<class UScoreElement> ScoreElementWidgetClass;
	TSubclassOf<class UNameInputWidget> NameInputWidgetClass;

	void PopulateHighScores(TArray<FHighScoreStruct> HighScores, int32 NewScore);
};
