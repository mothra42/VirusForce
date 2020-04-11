// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "VirusForceGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/VirusForcePawn.h"
#include "../NPC/MarkedVirusComponent.h"
#include "../Score/ScoreManager.h"
#include "../HUD/VirusForceHUD.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Life lost"))
	//decrement lives
	Lives--;
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
	//TODOS
	//reset spawn counter
	//respawn player
}

void AVirusForceGameMode::DestroyPawn(APawn* Pawn)
{
	auto Controller = Pawn->GetController();
	Controller->PawnPendingDestroy(Pawn);
	Controller->Destroy();
	//TODO make sure that any virus has any antibodies removed and is removed 
	//from marked viruses if it is one.
}