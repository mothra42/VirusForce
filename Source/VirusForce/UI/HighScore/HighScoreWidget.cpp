// Copyright Adam Farmer 2020


#include "HighScoreWidget.h"
#include "Components/VerticalBox.h"
#include "UObject/ConstructorHelpers.h"
#include "../../SaveGame/VirusForceSaveGame.h"
#include "ScoreElement.h"
#include "NameInputWidget.h"

UHighScoreWidget::UHighScoreWidget(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> ScoreElementClassFinder(TEXT("/Game/UI/Menu/HighScores/HighScoreElement"));
    ConstructorHelpers::FClassFinder<UUserWidget> NameInputWidgetClassFinder(TEXT("/Game/UI/Menu/HighScores/NameInputWidget"));
    ScoreElementWidgetClass = ScoreElementClassFinder.Class;
    NameInputWidgetClass = NameInputWidgetClassFinder.Class;
}

void UHighScoreWidget::Setup(TArray<FHighScoreStruct> HighScores)
{
    PopulateHighScores(HighScores);
    this->AddToViewport();
    UWorld* World = GetWorld();
    APlayerController* PlayerController = World->GetFirstPlayerController();
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UHighScoreWidget::PopulateHighScores(TArray<FHighScoreStruct> HighScores)
{   //TODO need to sort scores
    //check if new score is part of top ten scores, 
    //if it is then add it to viewport in proper place.
    //show top 10 scores
    for (int32 i = 0; i < HighScores.Num(); i++)
    {
        //UHighScoreWidget* HighScoreMenu = CreateWidget<UHighScoreWidget>(this, HighScoreWidgetClass);
        UScoreElement* ScoreElement = CreateWidget<UScoreElement>(this, ScoreElementWidgetClass);
        FText Rank = FText::FromString(FString::FromInt(i + 1));
        FText Name = FText::FromString(HighScores[i].PlayerName);
        FText Score = FText::FromString(FString::FromInt(HighScores[i].Score));
        ScoreElement->SetText(Rank, Name, Score);
        HighScoreList->AddChildToVerticalBox(ScoreElement);
    }

    UNameInputWidget* NameInputWidget = CreateWidget<UNameInputWidget>(this, NameInputWidgetClass);
    NameInputWidget->SetupWidget(FText::FromString(FString::FromInt(10000)));
    HighScoreList->AddChildToVerticalBox(NameInputWidget);
}