// Copyright Adam Farmer 2020


#include "Arena.h"
#include "DrawDebugHelpers.h"
#include "../NPC/Virus.h"
#include "../WaveDesign/WaveManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Public/EngineUtils.h"


// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//handles timing and what class of virus to spawn
	WaveManager = CreateDefaultSubobject<UWaveManager>(TEXT("WaveManagerComponent"));

	MinExtent = FVector(-6000, -6000, 0);
	MaxExtent = FVector(6000, 6000, 0);
}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaveManager->bNextWaveCanSpawn == true)
	{
		SpawnVirus();
	}
}

void AArena::SpawnVirus()
{
	FVector SpawnLocation;
	float VirusMeshRadius = WaveManager->CurrentlySpawningVirusType->GetDefaultObject<AVirus>()->MeshRadius;
	if (FindEmptyLocation(SpawnLocation, VirusMeshRadius))
	{
		PlaceVirus(SpawnLocation, WaveManager->CurrentlySpawningVirusType);
	}
}

bool AArena::bCanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation + FVector(1, 1, 1), //small offset so sweep will work
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(50.f)
		);

	return !HasHit;
}

bool AArena::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);
	const int32 MAX_ATTEMPTS = 25;

	for (int32 i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandLocation = FMath::RandPointInBox(Bounds);
		if (bCanSpawnAtLocation(RandLocation, Radius))
		{
			OutLocation = RandLocation;
			return true;
		}
	}
	return false;
}

void AArena::PlaceVirus(FVector SpawnPoint, TSubclassOf<AVirus> VirusClass)
{
	UWorld* World = GetWorld();
	if (World != NULL && VirusClass != nullptr)
	{
		FRotator RandRotator = UKismetMathLibrary::RandomRotator();
		AVirus* SpawnedVirus = World->SpawnActor<AVirus>(VirusClass, SpawnPoint, FRotator(0, RandRotator.Yaw, 0));
	}
}