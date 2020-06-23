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
	GetWorldTimerManager().SetTimer(
		TimerHandle_InfectionTimer,
		this,
		&AInfectableCell::ProduceViruses,
		IncubationTime
		);
}

void AInfectableCell::ProduceViruses()
{
	//Spawn more burst viruses or a new smaller type of virus with different behavior
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("I Was infected and now I'm blowing myself up"));
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