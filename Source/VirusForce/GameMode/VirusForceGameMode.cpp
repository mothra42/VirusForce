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

	LoadHighScore();

	//TODO set the save game object from the load to be used in the save so we can keep adding to the array
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
	if (UVirusForceSaveGame* SaveGameInstance = Cast<UVirusForceSaveGame>(UGameplayStatics::CreateSaveGameObject(UVirusForceSaveGame::StaticClass())))
	{
		SaveGameInstance->SaveHighScore(TEXT("TestPlayer"), ScoreManagerComponent->Score);
		SaveGameInstance->MyScore = ScoreManagerComponent->Score;

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("TestSlot"), 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Save Successful"));
		}
	}
}

void AVirusForceGameMode::LoadHighScore()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	USaveGameHelper* SaveGameHelper = NewObject<USaveGameHelper>();
	LoadedDelegate.BindUObject(SaveGameHelper, &USaveGameHelper::LoadGameDelegate);
	UGameplayStatics::AsyncLoadGameFromSlot(TEXT("TestSlot"), 0, LoadedDelegate);
	//if (UVirusForceSaveGame* LoadedGame = Cast<UVirusForceSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0)))
	//{
	//	// The operation was successful, so LoadedGame now contains the data we saved earlier.
	//	UE_LOG(LogTemp, Warning, TEXT("LOADED: %i"), LoadedGame->MyScore);
	//}
}