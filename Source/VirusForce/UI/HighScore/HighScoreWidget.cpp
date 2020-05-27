// Copyright Adam Farmer 2020


#include "HighScoreWidget.h"
#include "Components/VerticalBox.h"
#include "UObject/ConstructorHelpers.h"
#include "ScoreElement.h"

UHighScoreWidget::UHighScoreWidget(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> ScoreElementClassFinder(TEXT("/Game/UI/Menu/HighScores/HighScoreElement"));
    ScoreElementWidgetClass = ScoreElementClassFinder.Class;
}

void UHighScoreWidget::Setup(TArray<FHighScoreStruct> HighScores)
{
    PopulateHighScores(HighScores);
    UE_LOG(LogTemp, Warning, TEXT("Populated high scores"));
    this->AddToViewport();
    UWorld* World = GetWorld();
    APlayerController* PlayerController = World->GetFirstPlayerController();
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UHighScoreWidget::PopulateHighScores(TArray<FHighScoreStruct> HighScores)
{
    
    for (int32 i = 0; i < HighScores.Num(); i++)
    {
        //UHighScoreWidget* HighScoreMenu = CreateWidget<UHighScoreWidget>(this, HighScoreWidgetClass);
        UScoreElement* ScoreElement = CreateWidget<UScoreElement>(this, ScoreElementWidgetClass);
        FText Rank = FText::FromString(FString::FromInt(i + 1));
        //UE_LOG(LogTemp, Warning, TEXT("Player name is %s"), *HighScores[i].PlayerName)
        FText Name = FText::FromString(HighScores[i].PlayerName);
        FText Score = FText::FromString(FString::FromInt(HighScores[i].Score));
        ScoreElement->SetText(Rank, Name, Score);
        HighScoreList->AddChildToVerticalBox(ScoreElement);
    }
}