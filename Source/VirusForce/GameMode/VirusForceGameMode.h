// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VirusForceGameMode.generated.h"

UCLASS(MinimalAPI)
class AVirusForceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVirusForceGameMode();

	UFUNCTION(BlueprintCallable)
	class UMarkedVirusComponent* GetMarkedVirusComponent();

	class UScoreManager* GetScoreManagerComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VirusTracking)
	UMarkedVirusComponent* MarkedVirusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ScoreSystem)
	UScoreManager* ScoreManagerComponent;
};



