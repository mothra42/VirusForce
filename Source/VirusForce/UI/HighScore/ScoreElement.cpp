// Copyright Adam Farmer 2020


#include "ScoreElement.h"
#include "Components/TextBlock.h"

void UScoreElement::SetText(FText InRank, FText InName, FText InScore)
{
    Rank->Text = InRank;
    Name->Text = InName;
    Score->Text = InScore;
}