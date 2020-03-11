// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MarkedVirusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRUSFORCE_API UMarkedVirusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<class AVirus*> MarkedViruses;

public:	
	// Sets default values for this component's properties
	UMarkedVirusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<AVirus*> AddToMarkedViruses(AVirus* VirusToAdd);
	TArray<AVirus*> RemoveFromMarkedViruses(AVirus* VirusToRemove);
	TArray<AVirus*> GetMarkedViruses();

		
};
