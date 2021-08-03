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
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"

AVirusForceGameMode::AVirusForceGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AVirusForcePawn::StaticClass();

	MarkedVirusComponent = CreateDefaultSubobject<UMarkedVirusComponent>(TEXT("MarkedVirusComponent"));

	ScoreManagerComponent = CreateDefaultSubobject<UScoreManager>(TEXT("ScoreManagerComponent"));

	HUD = CreateDefaultSubobject<AVirusForceHUD>(TEXT("HUD"));

	Lives = MaxLives;
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
		PurgePlayfield();
		World->GetTimerManager().SetTimer(TimerHandle_LastDeathPause, this, &AVirusForceGameMode::DisplayHighScoreScreen, 1.3); //1.3 previous length
	}
	else
	{
		//decrement lives on death
		Lives--;
		//reset playfield and respawn player
		PurgePlayfield();

		World->GetTimerManager().SetTimer(TimerHandle_RespawnPlayer, this, &AVirusForceGameMode::RespawnPlayer, 1.5);
		World->GetTimerManager().PauseTimer(Arena->GetMassSpawnTimer());
		World->GetTimerManager().PauseTimer(Arena->GetSpawnTimer());
	}
}

void AVirusForceGameMode::PurgePlayfield(bool bSkipPlayer)
{
	MarkedVirusComponent->PurgeMarkedViruses();
	//destroy all pawns
	TArray<AActor*> ActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ActorArray);
	for (int32 i = 0; i < ActorArray.Num(); i++)
	{
		APawn* PawnToDestroy = Cast<APawn>(ActorArray[i]);
		AVirusForceProjectile* ProjectileToDestroy = Cast<AVirusForceProjectile>(ActorArray[i]);
		if (bSkipPlayer && Cast<AVirusForcePawn>(PawnToDestroy) != nullptr)
		{
			continue;
		}
		else if (PawnToDestroy != nullptr)
		{
			DestroyPawn(PawnToDestroy);
		}
		else if (ProjectileToDestroy != nullptr)
		{
			ProjectileToDestroy->Destroy();
		}
	}
}

void AVirusForceGameMode::ResetGame()
{
	Lives = MaxLives;
	ScoreManagerComponent->ResetScore();
}

void AVirusForceGameMode::DisplayHighScoreScreen()
{
	UVirusForceGameInstance* GameInstance = Cast<UVirusForceGameInstance>(GetGameInstance());
	if (GameInstance != nullptr && ScoreManagerComponent != nullptr)
	{
		GameInstance->DisplayHighScoreScreen(ScoreManagerComponent->Score);
		OnTransitionToHighScoreScreen.Broadcast();
	}
	//spawn new camera in high center of the arena and move view to that
	ACameraActor* ScoreScreenCamera = GetWorld()->SpawnActor<ACameraActor>(FVector(-143.28, -44.74, 3511.32), FRotator(-90.f, 0.f, 0.f));
	ScoreScreenCamera->GetCameraComponent()->bConstrainAspectRatio = false;
	PlayerController->SetViewTargetWithBlend(ScoreScreenCamera, 0.f);
	
	AVirusForcePawn* Player = Cast<AVirusForcePawn>(UGameplayStatics::GetActorOfClass(GetWorld(), GetPlayerPawnClass()));
	if (Player != nullptr)
	{
		Player->SetDestroyInNextGame();
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
		PreviousPlayerTransform = PlayerPawn->GetTransform();
		PlayerPawn->Destroy();
		//use deferred spawn to set projectile class before begin play is called
		AVirusForcePawn* NewPlayerPawn = GetWorld()->SpawnActorDeferred<AVirusForcePawn>(
			GetPlayerPawnClass(), 
			PreviousPlayerTransform
			);
		if (NewPlayerPawn != nullptr)
		{
			NewPlayerPawn->SetProjectile(LastUsedProjectile);
		}

		UGameplayStatics::FinishSpawningActor(NewPlayerPawn, PreviousPlayerTransform);
		PlayerController->Possess(NewPlayerPawn);
		GetWorld()->GetTimerManager().UnPauseTimer(Arena->GetMassSpawnTimer());
		GetWorld()->GetTimerManager().UnPauseTimer(Arena->GetSpawnTimer());
	}
}
