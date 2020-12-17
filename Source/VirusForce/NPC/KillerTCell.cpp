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
#include "../Player/VirusForceProjectile.h"
#include "../Score/ScoreManager.h"
#include "Virus.h"


// Sets default values
AKillerTCell::AKillerTCell()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CellWall(TEXT("/Game/Geometry/Meshes/PlayerMeshes/Player/CellWall"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Core(TEXT("/Game/Geometry/Meshes/PlayerMeshes/Player/Core"));
	// Create the mesh component
	CellWallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellWall"));
	CoreComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Core"));
	CoreComponent->SetupAttachment(CellWallComponent);
	//CoreComponent->AttachToComponent(CellWallComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("CoreSocket"));
	RootComponent = CellWallComponent;
	CellWallComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CellWallComponent->SetStaticMesh(CellWall.Object);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	// Movement
	MoveSpeed = 1000.0f;

}

// Called when the game starts or when spawned
void AKillerTCell::BeginPlay()
{
	Super::BeginPlay();
	CellWallComponent->OnComponentHit.AddDynamic(this, &AKillerTCell::OnHit);
	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		MarkedVirusComponent = GameMode->GetMarkedVirusComponent();
		NumberOfVirusesToConsume = MarkedVirusComponent->GetNumMarkedViruses();

		ScoreManager = GameMode->GetScoreManagerComponent();

		//in case the player calls a Killer T Cell before any virus are marked destroy self immediately
		DestroySelfWhenFinishedConsuming();
	}
}

// Called every frame
void AKillerTCell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AKillerTCell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ConsumeVirus(OtherActor);
}

void AKillerTCell::ConsumeVirus(AActor* ActorToConsume)
{
	if (MarkedVirusComponent != nullptr)
	{
		TArray<AActor*> OutAttachedActors;
		AVirus* Virus = Cast<AVirus>(ActorToConsume);
		if (Virus != nullptr)
		{
			if (MarkedVirusComponent->GetMarkedViruses().Find(Virus) >= 0)
			{
				IncreaseScoreForConsumingVirus(Virus);
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
					IncreaseScoreForConsumingVirus(HitProjectile->GetAttachedVirus());
					DestroyVirus(HitProjectile->GetAttachedVirus());
				}
			}
		}

		DestroySelfWhenFinishedConsuming();
	}
}

void AKillerTCell::DestroyVirus(AVirus* VirusToDestroy)
{
	auto AIController = VirusToDestroy->GetController();
	AIController->PawnPendingDestroy(VirusToDestroy);
	AIController->Destroy();
	VirusToDestroy->DestroyAttachedAntibodies();
	TArray<AVirus*>MarkedViruses = MarkedVirusComponent->RemoveFromMarkedViruses(VirusToDestroy);
	VirusToDestroy->Destroy();
}

void AKillerTCell::DestroySelfWhenFinishedConsuming()
{
	if (MarkedVirusComponent->GetMarkedViruses().Num() <= 0)
	{
		auto MyAIController = GetController();
		MyAIController->PawnPendingDestroy(this);
		MyAIController->Destroy();
		Destroy();
	}
}

void AKillerTCell::IncreaseScoreForConsumingVirus(AVirus* Virus)
{
	if (ScoreManager != nullptr)
	{
		ScoreManager->IncreaseScore(Virus, NumberOfVirusesToConsume);
	}
}