// Copyright Adam Farmer 2020


#include "MarkedVirusComponent.h"
#include "Virus.h"
#include "KillerTCell.h"


//Marked Virus Component is used to report a virus is marked and to keep track of marked viruses on screen
//Viruses will report themselves marked
//killer t cells will use a list of marked viruses to track and consume viruses

// Sets default values for this component's properties
UMarkedVirusComponent::UMarkedVirusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMarkedVirusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

TArray<AVirus*> UMarkedVirusComponent::AddToMarkedViruses(AVirus* VirusToAdd)
{
	if (VirusToAdd != nullptr)
	{
		MarkedViruses.Add(VirusToAdd);
	}
	return MarkedViruses;
}

TArray<AVirus*> UMarkedVirusComponent::RemoveFromMarkedViruses(AVirus* VirusToRemove)
{
	if (VirusToRemove != nullptr && MarkedViruses.Contains(VirusToRemove))
	{
		MarkedViruses.Remove(VirusToRemove);
	}
	return MarkedViruses;
}

TArray<AVirus*> UMarkedVirusComponent::GetMarkedViruses()
{
	return MarkedViruses;
}

int32 UMarkedVirusComponent::GetNumMarkedViruses()
{
	return MarkedViruses.Num();
}

void UMarkedVirusComponent::PurgeMarkedViruses()
{
	if (MarkedViruses.Num() >= 0)
	{
		MarkedViruses.Empty();
	}
}

void UMarkedVirusComponent::DistributeMarkedViruses(int32 NumKillerCells, TArray<AKillerTCell*> KillerTCellArray)
{
	if (KillerTCellArray.Num() > 0)
	{
		switch (NumKillerCells)
		{
		case 1:
		{
			KillerTCellArray[0]->MyMarkedViruses = MarkedViruses;
			if (KillerTCellArray[0]->ActorLocationPointer == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Null in Marked"))
			}
			MarkedViruses[0]->KillerTCellLocationPointer = KillerTCellArray[0]->ActorLocationPointer;

			if (MarkedViruses[0]->KillerTCellLocationPointer == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Still null"))
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Marked Virus Comp %s"), *MarkedViruses[0]->KillerTCellLocationPointer->ToString())
			}
			break;
		}
		case 2:
		{
			int32 HalfPoint = MarkedViruses.Num() / 2;
			for (int32 i = 0; i < HalfPoint; i++)
			{
				KillerTCellArray[0]->MyMarkedViruses.Add(MarkedViruses[i]);
			}
	
			for (int32 i = HalfPoint; i < MarkedViruses.Num(); i++)
			{
				KillerTCellArray[1]->MyMarkedViruses.Add(MarkedViruses[i]);
			}
			break;
		}
		case 3:
		{
			int32 ThirdPoint = MarkedViruses.Num() / 3;
			for (int32 i = 0; i < ThirdPoint; i++)
			{
				KillerTCellArray[0]->MyMarkedViruses.Add(MarkedViruses[i]);
			}
	
			for (int32 i = ThirdPoint; i < ThirdPoint + ThirdPoint; i++)
			{
				KillerTCellArray[1]->MyMarkedViruses.Add(MarkedViruses[i]);
			}
	
			for (int32 i = ThirdPoint + ThirdPoint; i < MarkedViruses.Num(); i++)
			{
				KillerTCellArray[2]->MyMarkedViruses.Add(MarkedViruses[i]);
			}
			break;
		}
		}
	}
}