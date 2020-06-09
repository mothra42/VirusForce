// Copyright Adam Farmer 2020


#include "NameInputWidget.h"
#include "../../SaveGame/VirusForceSaveGame.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UNameInputWidget::SetupWidget(FText InScore)
{
	Score->Text = InScore;
}