// Copyright Adam Farmer 2020


#include "ArcadeStyleTextInputBox.h"
#include "HighScoreWidget.h"

UArcadeStyleTextInputBox::UArcadeStyleTextInputBox()
{
	SetText(FText::FromString(AlphabetArray[AlphabetIndex]));
	ScrollRate = 0.4f;
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
		SetText(FText::FromString(PlayerName + AlphabetArray[AlphabetIndex]));
	}
	return AlphabetArray[AlphabetIndex];
}

void UArcadeStyleTextInputBox::ConfirmLetter()
{
	if (AlphabetArray[AlphabetIndex] == " END")
	{
		OnNameEntryCompleted.Broadcast(FText::FromString(PlayerName), ETextCommit::OnEnter);
		return;
	}
	if (PlayerName.Len() < NameCharLimit)
	{
		PlayerName += AlphabetArray[AlphabetIndex];
		SetText(FText::FromString(PlayerName));
	}
	AlphabetIndex;
}

void UArcadeStyleTextInputBox::DeleteLetter()
{
	if (PlayerName.Len() > 0)
	{
		PlayerName.RemoveAt(PlayerName.Len() - 1);
		SetText(FText::FromString(PlayerName));
		AlphabetIndex = 0;
	}
}

void UArcadeStyleTextInputBox::ScrollTimerExpired()
{
	bCanScroll = true;
}