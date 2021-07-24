// Copyright Adam Farmer 2020


#include "BurstVirus.h"
#include "../NeutralCells/InfectableCell.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../GameMode/VirusForceGameMode.h"
#include "../MarkedVirusComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EngineUtils.h"

void ABurstVirus::BeginPlay()
{
	Super::BeginPlay();

	Super::VirusMeshComponent->OnComponentHit.RemoveDynamic(this, &Super::OnHit);
	Super::VirusMeshComponent->OnComponentHit.AddDynamic(this, &ABurstVirus::OnHit);
	CheckWorldForInfectableCell();
}

void ABurstVirus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//clamp velocity to max speed if it is ever exceeded.
	float CurrentSpeed = Super::MovementComponent->Velocity.Size();
	if (CurrentSpeed > Super::MovementComponent->MaxSpeed)
	{
		FVector ClampedVelocity = Super::MovementComponent->Velocity.GetSafeNormal() * Super::MovementComponent->MaxSpeed;
		Super::MovementComponent->Velocity = ClampedVelocity;
	}
}

void ABurstVirus::CheckWorldForInfectableCell()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInfectableCell::StaticClass(), FoundActors);

	for (int32 i = 0; i < FoundActors.Num(); i++)
	{
		AInfectableCell* InfectableCell = Cast<AInfectableCell>(FoundActors[i]);
		if (InfectableCell != nullptr && !InfectableCell->InfectedStatus && !InfectableCell->ActorHasTag(FName("CapillaryCell")))
		{
			SetInfectableCell(InfectableCell);
			return;
		}
	}
}

void ABurstVirus::SetInfectableCell(AInfectableCell* InfectableCell)
{
	CellToInfect = InfectableCell;
	OnCellToInfectChanged.Broadcast(InfectableCell);
	//pick new random direction to start moving in
	SetNewRandomDirection();
}

void ABurstVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AInfectableCell* InfectableCell = Cast<AInfectableCell>(OtherActor);
	if (InfectableCell != nullptr && !InfectableCell->InfectedStatus)
	{
		InfectCell();
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ABurstVirus::InfectCell()
{
	CellToInfect->BeginInfection();
	AlertVirusesOnInfection();
	AVirusForceGameMode* GameMode = Cast<AVirusForceGameMode>(GetWorld()->GetAuthGameMode());
	UMarkedVirusComponent* MyMarkedVirusComponent = GameMode->GetMarkedVirusComponent();
	MyMarkedVirusComponent->RemoveFromMarkedViruses(this);
	DestroyAttachedAntibodies();
	Destroy();
}

void ABurstVirus::AlertVirusesOnInfection()
{
	TActorIterator<ABurstVirus> BurstVirusIterator = TActorIterator<ABurstVirus>(GetWorld());

	while (BurstVirusIterator)
	{
		//call function that adds this infectable cell to BurstVirus AI
		ABurstVirus* BurstVirus = *BurstVirusIterator;
		BurstVirus->OnCellToInfectChanged.Broadcast(nullptr);
		BurstVirus->SetNewRandomDirection();
		++BurstVirusIterator;
	}
}

void ABurstVirus::SetNewRandomDirection()
{
	FRotator RandRotator = UKismetMathLibrary::RandomRotator();
	SetActorRotation(FRotator(0.f, RandRotator.Yaw, GetActorRotation().Roll));
}