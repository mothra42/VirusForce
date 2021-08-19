// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../SaveGame/VirusForceSaveGame.h"
#include "HighScoreWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeFocusDelegate);


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
	//used to setup when a new score is being added
	void Setup(TArray<FHighScoreStruct> HighScores, int32 NewScore);
	//used when just viewing high scores

	UFUNCTION(BlueprintCallable)
	void Setup(TArray<FHighScoreStruct> HighScores);

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HighScoreList;

	UFUNCTION()
	void SaveOnPlayerNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UPROPERTY(Category = NameEntry, BlueprintCallable, BlueprintAssignable)
	FChangeFocusDelegate OnChangeFocus;

private:
	TSubclassOf<class UScoreElement> ScoreElementWidgetClass;
	TSubclassOf<class UNameInputWidget> NameInputWidgetClass;
	TSubclassOf<UUserWidget> TextButtonClass;


	void PopulateHighScores(TArray<FHighScoreStruct> HighScores, int32 NewScore);
	void SortScores(TArray<FHighScoreStruct>& HighScores);
};
