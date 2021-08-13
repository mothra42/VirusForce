// Copyright Adam Farmer 2020


#include "ArcadeStyleTextInputBox.h"

UArcadeStyleTextInputBox::UArcadeStyleTextInputBox()
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor called"));
	Text = FText::FromString(PlayerName);
	ScrollRate = 0.3f;
	ClearKeyboardFocusOnCommit = true;
}

FString UArcadeStyleTextInputBox::ScrollThroughLetters(bool IsUpInput)
{
	if (bCanScroll)
	{
		if (IsUpInput)
		{
			if (AlphabetIndex >= AlphabetArray.Num() - 1)
			{
				AlphabetIndex = 0;
			}
			else
			{
				AlphabetIndex++;
			}
		}
		else
		{
			if (AlphabetIndex <= 0)
			{
				AlphabetIndex = AlphabetArray.Num() - 1;
			}
			else
			{
				AlphabetIndex--;
			}
		}
		bCanScroll = false;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ScrollTimerExpired, this, &UArcadeStyleTextInputBox::ScrollTimerExpired, ScrollRate);
		UE_LOG(LogTemp, Warning, TEXT("Current letter is %s"), *AlphabetArray[AlphabetIndex]);
		SetText(FText::FromString(PlayerName + AlphabetArray[AlphabetIndex]));
	}
	return AlphabetArray[AlphabetIndex];
}

void UArcadeStyleTextInputBox::ConfirmLetter()
{
	if (AlphabetArray[AlphabetIndex] == "END")
	{
		HandleOnTextCommitted(FText::FromString(PlayerName), ETextCommit::OnEnter);
		return;
	}
	if (PlayerName.Len() < NameCharLimit)
	{
		PlayerName += AlphabetArray[AlphabetIndex];
		SetText(FText::FromString(PlayerName));
	}
}

void UArcadeStyleTextInputBox::DeleteLetter()
{
	PlayerName.RemoveAt(PlayerName.Len() - 1);
	SetText(FText::FromString(PlayerName));
}

void UArcadeStyleTextInputBox::ScrollTimerExpired()
{
	bCanScroll = true;
}