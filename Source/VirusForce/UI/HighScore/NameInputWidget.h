// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "NameInputWidget.generated.h"


struct FHighScoreStruct;
/**
 * 
 */
UCLASS()
class VIRUSFORCE_API UNameInputWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UArcadeStyleTextInputBox* ArcadeNameInput;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Rank;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Score;

    void SetupWidget(FText InRank, FText InScore);
};
