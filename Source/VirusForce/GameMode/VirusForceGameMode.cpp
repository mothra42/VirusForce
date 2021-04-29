// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VirusForceGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "../Player/VirusForcePawn.h"
#include "../Player/VirusForceProjectile.h"
#include "../NPC/MarkedVirusComponent.h"
#include "../Score/ScoreManager.h"
#include "../HUD/VirusForceHUD.h"
#include "../NPC/Virus.h"
#include "../Arena/Arena.h"
#include "../NPC/NeutralCells/InfectableCell.h"
#include "Blueprint/UserWidget.h"
#include "../UI/HighScore/HighScoreWidget.h"
#include "../GameInstance/VirusForceGameInstance.h"
#include "Components/EditableTextBox.h"
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

TSubclassOf<AVirusForcePawn> AVirusForceGameMode::GetPlayerPawnClass()
{
	return VirusForcePawn_BP;
}

void AVirusForceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AVirusForceGameMode::ResetGameOnLifeLost(UWorld* World)
{
	if (Lives == 0)
	{
		// reset playfield and transition to high score screen
		//UGameplayStatics::SetGamePaused(World, true);
		PurgePlayfield(World);
		World->GetTimerManager().SetTimer(TimerHandle_LastDeathPause, this, &AVirusForceGameMode::DisplayHighScoreScreen, 1.3);
	}
	else
	{
		//decrement lives on death
		Lives--;
		//reset playfield and respawn player
		PurgePlayfield(World);

		World->GetTimerManager().SetTimer(TimerHandle_RespawnPlayer, this, &AVirusForceGameMode::RespawnPlayer, 1.5);
		World->GetTimerManager().PauseTimer(Arena->GetMassSpawnTimer());
		World->GetTimerManager().PauseTimer(Arena->GetSpawnTimer());
	}
}

void AVirusForceGameMode::PurgePlayfield(UWorld* World)
{
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
}

void AVirusForceGameMode::DisplayHighScoreScreen()
{
	UVirusForceGameInstance* GameInstance = Cast<UVirusForceGameInstance>(GetGameInstance());
	if (GameInstance != nullptr && ScoreManagerComponent != nullptr)
	{
		GameInstance->DisplayHighScoreScreen(ScoreManagerComponent->Score);
		OnTransitionToHighScoreScreen.Broadcast();
	}
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
		PlayerPawn->StopPlayingMovementAudio();
		PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
		PlayerDeathTransform = PlayerPawn->GetActorTransform();
		PlayerPawn->SetPlayerInvisible();
		LastUsedProjectile = PlayerPawn->ProjectileClass;

		//spawn death particle emitter
		if (PlayerPawn->CellDeathEmitter != nullptr)
		{
			FTransform EmitterTransform = FTransform(FRotator(), PlayerPawn->GetActorLocation(), PlayerPawn->GetActorScale());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				PlayerPawn->CellDeathEmitter,
				EmitterTransform);
		}
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
