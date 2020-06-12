// Copyright Adam Farmer 2020


#include "VirusForceGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "../UI/HighScore/HighScoreWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
//TODO move a lot of logic with tracking lives and such from game mode to game instance.

UVirusForceGameInstance::UVirusForceGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> HighScoreClassFinder(TEXT("/Game/UI/Menu/HighScores/HighScoreScreen"));
	HighScoreWidgetClass = HighScoreClassFinder.Class;
}


void UVirusForceGameInstance::Init()
{
	Super::Init();
	if (SavedGame == nullptr)
	{
		LoadHighScores();
	}
}

void UVirusForceGameInstance::LoadHighScores()
{
	LoadedGame = Cast<UVirusForceSaveGame>(UGameplayStatics::CreateSaveGameObject(UVirusForceSaveGame::StaticClass()));
	LoadedGame->LoadSavedGame();
}

void UVirusForceGameInstance::DisplayHighScoreScreen(int32 Score)
{
	CreateHighScoreList();
	if (HighScoreWidget != nullptr)
	{
		HighScoreWidget->Setup(LoadedGame->HighScoreList, Score);
	}
	FinalScore = Score;
}

void UVirusForceGameInstance::CreateHighScoreList()
{
	HighScoreWidget = CreateWidget<UHighScoreWidget>(this, HighScoreWidgetClass);
}

void UVirusForceGameInstance::SaveHighScore(FText Name)
{
	//if saved game is already present add on to existing file.
	if (LoadedGame != nullptr)
	{
		LoadedGame->SaveHighScore(Name.ToString(), FinalScore);
		//update HighScoreList with latest high score;
		//HighScoreList = LoadedGame->HighScoreList;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("At Time of call saved game was null, no game was saved"));
	}
}