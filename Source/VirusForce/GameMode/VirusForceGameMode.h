// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VirusForceGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTransitionToHighScoreScreen);

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

	TSubclassOf<class AVirusForcePawn> GetPlayerPawnClass();

	void ResetGameOnLifeLost(UWorld* World);

	FORCEINLINE void SetArena(AArena* InArena) { Arena = InArena; }

	UPROPERTY(Category = HighScoreUI, BlueprintAssignable)
	FTransitionToHighScoreScreen OnTransitionToHighScoreScreen;

	UFUNCTION(BlueprintCallable)
	void PurgePlayfield();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VirusTracking)
	UMarkedVirusComponent* MarkedVirusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ScoreSystem)
	UScoreManager* ScoreManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	AVirusForceHUD* HUD;

	UPROPERTY(EditDefaultsOnly, Category = PlayerRespawn)
	TSubclassOf<class AVirusForcePawn> VirusForcePawn_BP;

	virtual void BeginPlay() override;

private:
	class AVirusForcePawn* PlayerPawn;

	class APlayerController* PlayerController;

	class UVirusForceSaveGame* SavedGame;

	TSubclassOf<class AVirusForceProjectile> LastUsedProjectile;
	
	FTransform PlayerDeathTransform;

	FTimerHandle TimerHandle_RespawnPlayer;

	FTimerHandle TimerHandle_LastDeathPause;

	void DisplayHighScoreScreen();

	void DestroyPawn(APawn* Pawn);

	void RespawnPlayer();
};



