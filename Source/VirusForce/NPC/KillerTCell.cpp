// Copyright Adam Farmer 2020


#include "KillerTCell.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MarkedVirusComponent.h"
#include "../VirusForceGameMode.h"

// Sets default values
AKillerTCell::AKillerTCell()
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

}

// Called when the game starts or when spawned
void AKillerTCell::BeginPlay()
{
	Super::BeginPlay();

	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		MarkedVirusComponent = GameMode->GetMarkedVirusComponent();
	}
}

// Called every frame
void AKillerTCell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

