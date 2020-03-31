// Copyright Adam Farmer 2020


#include "KillerTCell.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MarkedVirusComponent.h"
#include "../GameMode/VirusForceGameMode.h"
#include "Virus.h"
#include "../Player/VirusForceProjectile.h"

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

//TODO Create overlap function that will consume marked viruses only

void AKillerTCell::ConsumeVirus(AActor* ActorToConsume)
{
	//TODO killer t cell is having trouble consuming virus need to check when the on hit is failing.
	TArray<AActor*> OutAttachedActors;
	AVirus* Virus = Cast<AVirus>(ActorToConsume);
	if (Virus != nullptr)
	{
		if (MarkedVirusComponent->GetMarkedViruses().Find(Virus) >= 0)
		{
			DestroyVirus(Virus);
		}
	}

	//if the hit actor is not a virus check if it is a projectile attached to a virus
	else
	{
		AVirusForceProjectile* HitProjectile = Cast<AVirusForceProjectile>(ActorToConsume);
		if (HitProjectile != nullptr)
		{
			if (HitProjectile->IsAttached)
			{
				DestroyVirus(HitProjectile->GetAttachedVirus());
			}
		}
	}
	
	if (MarkedVirusComponent->GetMarkedViruses().Num() <= 0)
	{
		Destroy();
	}
}

void AKillerTCell::DestroyVirus(AVirus* VirusToDestroy)
{
	TArray<AActor*> OutAttachedActors;

	auto AIController = VirusToDestroy->GetController();
	AIController->PawnPendingDestroy(VirusToDestroy);
	AIController->Destroy();
	TArray<AVirus*>MarkedViruses = MarkedVirusComponent->RemoveFromMarkedViruses(VirusToDestroy);
	VirusToDestroy->GetAttachedActors(OutAttachedActors);
	for (int32 i = 0; i < OutAttachedActors.Num(); i++)
	{
		OutAttachedActors[i]->Destroy();
	}
	VirusToDestroy->Destroy();
}