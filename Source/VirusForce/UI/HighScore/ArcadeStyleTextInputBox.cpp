// Copyright Adam Farmer 2020


#include "ArcadeStyleTextInputBox.h"
#include "HighScoreWidget.h"

UArcadeStyleTextInputBox::UArcadeStyleTextInputBox()
{
	PlayerName = AlphabetArray[AlphabetIndex];
	ScrollRate = 0.4f;
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
		AppendNonConfirmedChar(AlphabetArray[AlphabetIndex]);
		//SetText(FText::FromString(PlayerName + AlphabetArray[AlphabetIndex]));
	}
	return AlphabetArray[AlphabetIndex];
}

void UArcadeStyleTextInputBox::HandleKeyboardInput(bool IsValidKey, FKey KeyType)
{
	//Ignore if not a valid key press
	if (IsValidKey)
	{
		if (KeyType == EKeys::W || KeyType == EKeys::S)
		{
			ScrollThroughLetters(KeyType == EKeys::W);
		}
		else if (KeyType == EKeys::Enter || KeyType == EKeys::SpaceBar)
		{
			ConfirmLetter();
		}
		else if (KeyType == EKeys::Delete || KeyType == EKeys::BackSpace)
		{
			DeleteLetter();
		}
	}
}

void UArcadeStyleTextInputBox::ConfirmLetter()
{
	if (AlphabetArray[AlphabetIndex] == " END" && PlayerName != " END" && PlayerName.Len() >= 3)
	{
		PlayerName.RemoveFromEnd(" END");
		OnNameEntryCompleted.Broadcast(FText::FromString(PlayerName), ETextCommit::OnEnter);
		return;
	}
	if (PlayerName.Len() < NameCharLimit)
	{
		PlayerName += AlphabetArray[AlphabetIndex];
	}
	AlphabetIndex;
}

void UArcadeStyleTextInputBox::DeleteLetter()
{
	if (PlayerName.Len() > 0)
	{
		PlayerName.RemoveAt(PlayerName.Len() - 1);
		AlphabetIndex = 0;
	}
}

void UArcadeStyleTextInputBox::ScrollTimerExpired()
{
	bCanScroll = true;
}

FString UArcadeStyleTextInputBox::AppendNonConfirmedChar(FString CharToAppend)
{
	if (!PlayerName.RemoveFromEnd(" END") && PlayerName.Len() > 0)
	{
		PlayerName.RemoveAt(PlayerName.Len() - 1);
	}
	PlayerName += CharToAppend;

	return PlayerName;
}