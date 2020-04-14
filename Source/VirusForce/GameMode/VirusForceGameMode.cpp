// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VirusForceGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "../Player/VirusForcePawn.h"
#include "../NPC/MarkedVirusComponent.h"
#include "../Score/ScoreManager.h"
#include "../HUD/VirusForceHUD.h"
#include "../NPC/Virus.h"

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

void AVirusForceGameMode::ResetGameOnLifeLost(UWorld* World)
{
	//decrement lives
	Lives--;

	//Reset MarkedVirusesArray
	MarkedVirusComponent->PurgeMarkedViruses();

	//destroy all pawns
	TArray<AActor*> VirusArray;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), VirusArray);
	for (int32 i = 0; i < VirusArray.Num(); i++)
	{
		APawn* PawnToDestroy = Cast<APawn>(VirusArray[i]);
		if (PawnToDestroy != nullptr)
		{
			DestroyPawn(PawnToDestroy);
		}
	}
	//respawn player
	AVirusForcePawn* Player = World->SpawnActor<AVirusForcePawn>(DefaultPawnClass, FVector(0, 0, 0), FRotator(0, 0, 0));
	//Player->AutoPossessPlayer()
	if (PlayerController != nullptr)
	{
		PlayerController->Possess(Player);
	}
	//TODO test game with resetting spawn counter
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
		PlayerController = Cast<APlayerController>(Pawn->GetController());
	}
	AVirus* Virus = Cast<AVirus>(Pawn);
	if (Virus != nullptr)
	{
		Virus->DestroyAttachedAntibodies();
	}
	Pawn->Destroy();
}