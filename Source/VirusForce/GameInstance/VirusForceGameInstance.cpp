// Copyright Adam Farmer 2020


#include "VirusForceGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "../UI/HighScore/HighScoreWidget.h"

//TODO move a lot of logic with tracking lives and such from game mode to game instance.

UVirusForceGameInstance::UVirusForceGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> HighScoreClassFinder(TEXT("/Game/UI/Menu/HighScores/HighScoreScreen"));
	HighScoreWidgetClass = HighScoreClassFinder.Class;
}

UHighScoreWidget* UVirusForceGameInstance::CreateHighScoreList()
{
	UHighScoreWidget* HighScoreMenu = CreateWidget<UHighScoreWidget>(this, HighScoreWidgetClass);
	return HighScoreMenu;
}
