// Fill out your copyright notice in the Description page of Project Settings.


#include "Virus.h"
#include "MarkedVirusComponent.h"
#include "../GameMode/VirusForceGameMode.h"
#include "../Player/VirusForcePawn.h"
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

	ShipMeshComponent->OnComponentHit.AddDynamic(this, &AVirus::OnHit);

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

void AVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AVirusForcePawn* Player = Cast<AVirusForcePawn>(OtherActor);
	if (Player)
	{
		Player->LoseLife();
	}
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

int32 AVirus::NumOfAttachedAntibodies()
{
	TArray<AActor*> AttachedAntibodies;
	GetAttachedActors(AttachedAntibodies);

	return AttachedAntibodies.Num();
}

void AVirus::DestroyAttachedAntibodies()
{
	TArray<AActor*> OutAttachedActors;
	GetAttachedActors(OutAttachedActors);
	for (int32 i = 0; i < OutAttachedActors.Num(); i++)
	{
		OutAttachedActors[i]->Destroy();
	}
}
