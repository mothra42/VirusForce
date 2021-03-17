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
	CellWallComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillerTCell::OnOverlapBegin);

	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		MarkedVirusComponent = GameMode->GetMarkedVirusComponent();
		NumberOfVirusesToConsume = MarkedVirusComponent->GetNumMarkedViruses();

		ScoreManager = GameMode->GetScoreManagerComponent();
	}

	ActorLocationPointer = &ActorLocation;
}

// Called every frame
void AKillerTCell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ActorLocation = GetActorLocation();
	if (ActorLocationPointer != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Working? %s"), *ActorLocationPointer->ToString());
	}
}

void AKillerTCell::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ConsumeVirus(OtherActor);
}

void AKillerTCell::ConsumeVirus(AActor* ActorToConsume)
{
	if (MarkedVirusComponent != nullptr)
	{
		TArray<AActor*> OutAttachedActors;
		AVirus* Virus = Cast<AVirus>(ActorToConsume);
		if (Virus->KillerTCellLocationPointer != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Killer T Cell is %s"), *Virus->KillerTCellLocationPointer->ToString())
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FUCK"));
		}
		
		if (Virus != nullptr)
		{
			if (MyMarkedViruses.Find(Virus) >= 0)
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
	RemoveVirusFromMarkedViruses(VirusToDestroy);
	VirusToDestroy->Destroy();
}

void AKillerTCell::DestroySelfWhenFinishedConsuming()
{
	if (MyMarkedViruses.Num() <= 0)
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

TArray<AVirus*> AKillerTCell::RemoveVirusFromMarkedViruses(AVirus* VirusToRemove)
{
	if (VirusToRemove != nullptr && MyMarkedViruses.Contains(VirusToRemove))
	{
		MyMarkedViruses.Remove(VirusToRemove);
	}
	return MyMarkedViruses;
}

//TODO Create "animation" of killer cell rising from starting point, can swirl around FVector(0.f, 0.f, 0.f);