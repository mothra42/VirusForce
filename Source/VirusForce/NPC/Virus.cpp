// Fill out your copyright notice in the Description page of Project Settings.


#include "Virus.h"
#include "MarkedVirusComponent.h"
#include "../GameMode/VirusForceGameMode.h"
#include "../Player/VirusForcePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AVirus::AVirus()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VirusMesh(TEXT("/Game/Geometry/Meshes/VirusMeshes/StochasticVirus/StochasticVirus"));
	// Create the mesh component
	VirusMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VirusMesh"));
	RootComponent = VirusMeshComponent;
	VirusMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	VirusMeshComponent->SetStaticMesh(VirusMesh.Object);
	VirusMeshComponent->bIgnoreRadialForce = true;
	VirusMeshComponent->bIgnoreRadialImpulse = true;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	BurstParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BurstParticleEffect"));
	BurstParticleEffect->AttachTo(VirusMeshComponent);

	// Movement
	MoveSpeed = 1000.0f;

	// Mesh Radius used for actor placement
	MeshRadius = 65.0f;
}

// Called when the game starts or when spawned
void AVirus::BeginPlay()
{
	Super::BeginPlay();
	AvailableSocketNames = VirusMeshComponent->GetAllSocketNames();

	VirusMeshComponent->OnComponentHit.AddDynamic(this, &AVirus::OnHit);
	//get marked virus component from game mode
	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		MarkedVirusComponent = GameMode->GetMarkedVirusComponent();
	}

	//Setup mesh and collision for spawn in
	SetActorEnableCollision(false);
	VirusMeshComponent->SetRelativeScale3D(VirusScale);

	BurstParticleEffect->Activate();
}

// Called every frame
void AVirus::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto Rotation = GetActorRotation();
	SetActorRotation(FRotator(0.f, Rotation.Yaw, Rotation.Roll));
	if (!IsVirusReady)
	{
		SpawnInAnimation();
	}
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

void AVirus::SpawnInAnimation()
{
	VirusMeshComponent->SetRelativeScale3D(VirusScale);
	if (VirusMeshComponent->GetRelativeScale3D().Size() >= FVector(1.f).Size())
	{
		SetVirusReady();
	}
	VirusScale += FVector(0.01);
}

void AVirus::SetVirusReady()
{
	SetActorEnableCollision(true);
	IsVirusReady = true;
}

FVector AVirus::GetKillerTCellLocation()
{
	if (KillerTCellLocationPointer != nullptr)
	{
		return *KillerTCellLocationPointer;
	}

	//if killer t cell pointer is not set, set the value to something far outside the arena
	return FVector(0.f, 0.f, -10000.f);
}

void AVirus::AlterMovementSpeed()
{
	MovementComponent->MaxSpeed *= SpeedModifier;
}