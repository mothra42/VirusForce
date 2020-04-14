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
	UFUNCTION(BlueprintCallable)
	TArray<AVirus*> AddToMarkedViruses(AVirus* VirusToAdd);

	UFUNCTION(BlueprintCallable)
	TArray<AVirus*> RemoveFromMarkedViruses(AVirus* VirusToRemove);

	UFUNCTION(BlueprintCallable)
	TArray<AVirus*> GetMarkedViruses();

	int32 GetNumMarkedViruses();

	void PurgeMarkedViruses();
};
