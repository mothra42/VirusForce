// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VirusForceGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../Player/VirusForcePawn.h"
#include "../Player/VirusForceProjectile.h"
#include "../NPC/MarkedVirusComponent.h"
#include "../Score/ScoreManager.h"
#include "../HUD/VirusForceHUD.h"
#include "../NPC/Virus.h"
#include "../Arena/Arena.h"
#include "../SaveGame/VirusForceSaveGame.h"
#include "../SaveGame/SaveGameHelper.h"
#include "TimerManager.h"

AVirusForceGameMode::AVirusForceGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AVirusForcePawn::StaticClass();

	MarkedVirusComponent = CreateDefaultSubobject<UMarkedVirusComponent>(TEXT("MarkedVirusComponent"));

	ScoreManagerComponent = CreateDefaultSubobject<UScoreManager>(TEXT("ScoreManagerComponent"));

	HUD = CreateDefaultSubobject<AVirusForceHUD>(TEXT("HUD"));
}

UMarkedVirusComponent* AVirusForceGameMode::GetMarkedVirusComponent()
{
	return MarkedVirusComponent;
}

UScoreManager* AVirusForceGameMode::GetScoreManagerComponent()
{
	return ScoreManagerComponent;
}

AVirusForceHUD* AVirusForceGameMode::GetHUDComponent()
{
	return HUD;
}

void AVirusForceGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (SavedGame == nullptr)
	{
		SavedGame = LoadHighScoreSync();
	}
}

void AVirusForceGameMode::ResetGameOnLifeLost(UWorld* World)
{
	//decrement lives
	Lives--;

	if (Lives < 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("StartScreen"));
		SaveHighScore();
	}
	//Reset MarkedVirusesArray
	MarkedVirusComponent->PurgeMarkedViruses();

	//destroy all pawns
	TArray<AActor*> ActorArray;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), ActorArray);
	for (int32 i = 0; i < ActorArray.Num(); i++)
	{
		APawn* PawnToDestroy = Cast<APawn>(ActorArray[i]);
		AVirusForceProjectile* ProjectileToDestroy = Cast<AVirusForceProjectile>(ActorArray[i]);
		if (PawnToDestroy != nullptr)
		{
			DestroyPawn(PawnToDestroy);
		}
		else if (ProjectileToDestroy != nullptr)
		{
			ProjectileToDestroy->Destroy();
		}
	}
	
	World->GetTimerManager().SetTimer(TimerHandle_RespawnPlayer, this, &AVirusForceGameMode::RespawnPlayer, 2.f);
	World->GetTimerManager().PauseTimer(Arena->GetMassSpawnTimer());
	World->GetTimerManager().PauseTimer(Arena->GetSpawnTimer());
}

void AVirusForceGameMode::DestroyPawn(APawn* Pawn)
{
	if (Cast<AVirusForcePawn>(Pawn) == nullptr)
	{
		auto Controller = Pawn->GetController();
		Controller->PawnPendingDestroy(Pawn);
		Controller->Destroy();
	}
	else
	{
		PlayerPawn = Cast<AVirusForcePawn>(Pawn);
		PlayerPawn->DisableMovement();
		PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
		PlayerDeathTransform = PlayerPawn->GetActorTransform();
		PlayerPawn->GetShipMeshComponent()->SetVisibility(false);
		LastUsedProjectile = PlayerPawn->ProjectileClass;
		return;
	}

	AVirus* Virus = Cast<AVirus>(Pawn);
	if (Virus != nullptr)
	{
		Virus->DestroyAttachedAntibodies();
	}

	Pawn->Destroy();
}

void AVirusForceGameMode::RespawnPlayer()
{
	if (PlayerController != nullptr && PlayerPawn != nullptr)
	{
		PlayerPawn->Destroy();
		RestartPlayerAtTransform(PlayerController, PlayerDeathTransform);
		AVirusForcePawn* NewPlayerPawn = Cast<AVirusForcePawn>(PlayerController->GetPawn());
		if (NewPlayerPawn != nullptr)
		{
			NewPlayerPawn->SetProjectile(LastUsedProjectile);
		}
		GetWorld()->GetTimerManager().UnPauseTimer(Arena->GetMassSpawnTimer());
		GetWorld()->GetTimerManager().UnPauseTimer(Arena->GetSpawnTimer());
	}
}

void AVirusForceGameMode::SaveHighScore()
{
	if (SavedGame != nullptr)
	{
		DelegateAsyncSave();
	}
	else
	{
		SavedGame = Cast<UVirusForceSaveGame>(UGameplayStatics::CreateSaveGameObject(UVirusForceSaveGame::StaticClass()));
		DelegateAsyncSave();
	}
}

void AVirusForceGameMode::DelegateAsyncSave()
{
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	USaveGameHelper* SaveGameHelper = NewObject<USaveGameHelper>();
	SavedDelegate.BindUObject(SaveGameHelper, &USaveGameHelper::SaveGameDelegate);
	SavedGame->SaveHighScore(TEXT("TestPlayer"), ScoreManagerComponent->Score);
	UGameplayStatics::AsyncSaveGameToSlot(SavedGame, TEXT("TestSlot"), 0, SavedDelegate);
}

void AVirusForceGameMode::LoadHighScoreAsync()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	USaveGameHelper* SaveGameHelper = NewObject<USaveGameHelper>();
	LoadedDelegate.BindUObject(SaveGameHelper, &USaveGameHelper::LoadGameDelegate);
	UGameplayStatics::AsyncLoadGameFromSlot(TEXT("TestSlot"), 0, LoadedDelegate);
}

UVirusForceSaveGame* AVirusForceGameMode::LoadHighScoreSync()
{
	if (UVirusForceSaveGame* LoadedGame = Cast<UVirusForceSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0)))
	{
		return LoadedGame;
	}

	//if no save object found create a new one
	return Cast<UVirusForceSaveGame>(UGameplayStatics::CreateSaveGameObject(UVirusForceSaveGame::StaticClass()));
}