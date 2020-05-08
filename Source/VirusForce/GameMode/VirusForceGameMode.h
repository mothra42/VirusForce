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

	UPROPERTY(Category = Lives, EditDefaultsOnly, BlueprintReadOnly)
	int32 Lives = 3;

	class AArena* Arena;

	UFUNCTION(BlueprintCallable)
	class UMarkedVirusComponent* GetMarkedVirusComponent();

	UFUNCTION(BlueprintCallable)
	class UScoreManager* GetScoreManagerComponent();

	class AVirusForceHUD* GetHUDComponent();

	void ResetGameOnLifeLost(UWorld* World);

	FORCEINLINE void SetArena(AArena* InArena) { Arena = InArena; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VirusTracking)
	UMarkedVirusComponent* MarkedVirusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ScoreSystem)
	UScoreManager* ScoreManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	AVirusForceHUD* HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerRespawn)
	TSubclassOf<class AVirusForcePawn> VirusForcePawn_BP;

private:
	class AVirusForcePawn* PlayerPawn;

	class APlayerController* PlayerController;

	TSubclassOf<class AVirusForceProjectile> LastUsedProjectile;
	
	FTransform PlayerDeathTransform;

	FTimerHandle TimerHandle_RespawnPlayer;

	void DestroyPawn(APawn* Pawn);

	void RespawnPlayer();
};



