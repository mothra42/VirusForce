// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VirusForceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API UVirusForceGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UVirusForceGameInstance();

private:
	TSubclassOf<class UHighScoreWidget> HighScoreWidgetClass;

public:
	class UHighScoreWidget* CreateHighScoreList();
};
