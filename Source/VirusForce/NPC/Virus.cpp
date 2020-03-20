// Fill out your copyright notice in the Description page of Project Settings.


#include "Virus.h"
#include "MarkedVirusComponent.h"
#include "../GameMode/VirusForceGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AVirus::AVirus()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	// Movement
	MoveSpeed = 1000.0f;

	// Mesh Radius used for actor placement
	MeshRadius = 65.0f;
}

// Called when the game starts or when spawned
void AVirus::BeginPlay()
{
	Super::BeginPlay();

	AvailableSocketNames = ShipMeshComponent->GetAllSocketNames();

	//get marked virus component from game mode
	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		MarkedVirusComponent = GameMode->GetMarkedVirusComponent();
	}
}

// Called every frame
void AVirus::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}


bool AVirus::AddToMarkedViruses()
{
	//if virus is unmarked and MarkedVirusComponent is valid, add to list
	if (!IsMarked && MarkedVirusComponent != nullptr)
	{
		//add self to marked virus list when marked
		MarkedVirusComponent->AddToMarkedViruses(this);
		IsMarked = true;
		return true;
	}

	return false;
}
