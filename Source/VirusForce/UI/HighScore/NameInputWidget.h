// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
    UPROPERTY(meta = (BindWidget))
    class UEditableText* Name;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Score;

    void SetupWidget(FText InScore);
};
