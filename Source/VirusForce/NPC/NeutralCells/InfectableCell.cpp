// Copyright Adam Farmer 2020


#include "InfectableCell.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "EngineUtils.h"
#include "../Viruses/BurstVirus.h"
#include "../Virus.h"

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

	AlertVirusesOnSpawn();
}

// Called every frame
void AInfectableCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//set timer for infection process
void AInfectableCell::BeginInfection()
{
	InfectedStatus = true;
	AlertVirusesOnInfection();
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
	Destroy();
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

void AInfectableCell::AlertVirusesOnInfection()
{
	OnVirusInfection.Broadcast();
}