// Copyright Adam Farmer 2020


#include "InfectableCell.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "EngineUtils.h"
#include "../Viruses/BurstVirus.h"
#include "../Virus.h"
#include "Components/SplineComponent.h"
#include "../../GameBackground/ArteryJunctionComponent.h"
#include "../../Arena/Arena.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../GameMode/VirusForceGameMode.h"

// Sets default values
AInfectableCell::AInfectableCell()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInfectableCell::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		TimerHandle_AlertVirusTimer,
		this,
		&AInfectableCell::AlertVirusesOnSpawn,
		TimeBeforeAlertViruses
		);
}

// Called every frame
void AInfectableCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//set timer for infection process
void AInfectableCell::BeginInfection()
{
	//check to see if the cell is overlapping the arena when it is infected
	//if it is abort infection process.
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	for (int32 i = 0; i < OverlappingActors.Num(); i++)
	{
		if (Cast<AArena>(OverlappingActors[i]) != nullptr)
		{
			return;
		}
	}

	InfectedStatus = true;
	OnVirusInfection.Broadcast();
	GetWorldTimerManager().SetTimer(
		TimerHandle_InfectionTimer,
		this,
		&AInfectableCell::ProduceViruses,
		IncubationTime
		);
}

//spawns more viruses once infection is completed
void AInfectableCell::ProduceViruses()
{
	SetActorEnableCollision(false);
	FVector SpawnLocation = this->GetActorLocation();
	UWorld* World = GetWorld();
	int32 Yaw = 0;
	//spawns viruses facing 45 degrees away from each other.
	for (int32 i = 0; i < 7; i++)
	{
		World->SpawnActor<AVirus>(VirusTypeToSpawn, SpawnLocation, FRotator(0, Yaw, 0));
		Yaw += 45;
	}
	//destroy is now called in the blueprint at the end of the disintegration timeline
}

void AInfectableCell::AlertVirusesOnSpawn()
{
	TActorIterator<ABurstVirus> BurstVirusIterator = TActorIterator<ABurstVirus>(GetWorld());

	while (BurstVirusIterator)
	{
		//call function that adds this infectable cell to BurstVirus AI
		ABurstVirus* BurstVirus = *BurstVirusIterator;
		BurstVirus->SetInfectableCell(this);
		++BurstVirusIterator;
	}
}

USplineComponent* AInfectableCell::GetNewArtery(UPrimitiveComponent* OverlappedJunctionMesh, USplineComponent* CurrentArtery)
{
	auto ArteryJunction = OverlappedJunctionMesh->GetAttachParent();
	UArteryJunctionComponent* OverlappedJunction = Cast<UArteryJunctionComponent>(ArteryJunction);
	if (OverlappedJunction != nullptr)
	{
		for (size_t i = 0; i < OverlappedJunction->ArteryArray.Num(); i++)
		{
			if (OverlappedJunction->ArteryArray[i] != CurrentArtery)
			{
				int32 NumSplinePoints = OverlappedJunction->ArteryArray[i]->GetNumberOfSplinePoints();
				int32 NewSplinePoint = OverlappedJunction->ArteryLocationMap[OverlappedJunction->ArteryArray[i]];
				int32 CurrentSplinePoint = OverlappedJunction->ArteryLocationMap[CurrentArtery];
				//check if the other spline point is the end of that spline, if it is we will ignore it
				if (NewSplinePoint < NumSplinePoints - 1)
				{
					if (CurrentSplinePoint == CurrentArtery->GetNumberOfSplinePoints() - 1)
					{
						return OverlappedJunction->ArteryArray[i];
					}
					else if (UKismetMathLibrary::RandomBoolWithWeight(0.3))
					{
						return OverlappedJunction->ArteryArray[i];
					}
				}
				return nullptr;
			}
		}
	}
	return nullptr;
}

float AInfectableCell::GetTimeOffset(USplineComponent* Artery, UPrimitiveComponent* ArteryJunctionMesh)
{
	auto ArteryJunction = ArteryJunctionMesh->GetAttachParent();
	UArteryJunctionComponent* OverlappedJunction = Cast<UArteryJunctionComponent>(ArteryJunction);
	if (OverlappedJunction != nullptr)
	{
		int32 NumSplinePoints = Artery->GetNumberOfSplinePoints();
		int32 SplinePoint = OverlappedJunction->ArteryLocationMap[Artery];
		if (SplinePoint == 0)
		{
			return 0.0;
		}
		float DistanceAtPoint = Artery->GetDistanceAlongSplineAtSplinePoint(SplinePoint);
		float TimeOffset = DistanceAtPoint / Artery->GetSplineLength();
		return TimeOffset;
	}
	return 0.0;
}