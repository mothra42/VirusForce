// Copyright Adam Farmer 2020


#include "MarkedVirusComponent.h"
#include "Virus.h"


//Marked Virus Component is used to report a virus is marked and to keep track of marked viruses on screen
//Viruses will report themselves marked
//killer t cells will use a list of marked viruses to track and consume viruses

// Sets default values for this component's properties
UMarkedVirusComponent::UMarkedVirusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMarkedVirusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMarkedVirusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	if (VirusToRemove != nullptr)
	{
		MarkedViruses.Remove(VirusToRemove);
	}
	return MarkedViruses;
}

TArray<AVirus*> UMarkedVirusComponent::GetMarkedViruses()
{
	return MarkedViruses;
}
