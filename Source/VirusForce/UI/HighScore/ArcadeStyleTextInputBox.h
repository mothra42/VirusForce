// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "ArcadeStyleTextInputBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNameEntryCompletedDelegate, const FText&, Text, ETextCommit::Type, CommitMethod);

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API UArcadeStyleTextInputBox : public UEditableTextBox
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = NameEntry, BlueprintCallable)
	FOnNameEntryCompletedDelegate OnNameEntryCompleted;

	UArcadeStyleTextInputBox();

	UFUNCTION(BlueprintCallable)
	FString ScrollThroughLetters(bool IsUpInput);

	UFUNCTION(BlueprintCallable)
	void HandleKeyboardInput(bool IsValidKey, FText KeyName);

	UFUNCTION(BlueprintCallable)
	void ConfirmLetter();

	UFUNCTION(BlueprintCallable)
	void DeleteLetter();

	UPROPERTY(Category = UI, EditAnywhere, BlueprintReadWrite)
	float ScrollRate;

	void ScrollTimerExpired();

	FORCEINLINE FString GetPlayerName() const { return PlayerName; }

private:
	TArray<FString> AlphabetArray = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
	"O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", " END"};

	int32 AlphabetIndex = 0;

	bool bCanScroll = true;

	FTimerHandle TimerHandle_ScrollTimerExpired;

	FString PlayerName;

	UPROPERTY(Category = NameLimits, EditAnywhere)
	int32 NameCharLimit = 10;
};
