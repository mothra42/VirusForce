// Copyright Adam Farmer 2020


#include "BurstVirus.h"
#include "../NeutralCells/InfectableCell.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ABurstVirus::BeginPlay()
{
	Super::BeginPlay();

	Super::VirusMeshComponent->OnComponentHit.RemoveDynamic(this, &Super::OnHit);
	Super::VirusMeshComponent->OnComponentHit.AddDynamic(this, &ABurstVirus::OnHit);
	CheckWorldForInfectableCell();
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
			InfectableCell->OnVirusInfection.AddDynamic(this, &ABurstVirus::CheckWorldForInfectableCell);
			return;
		}
	}

	//if no infectablevirus is found
	SetInfectableCell(nullptr);
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
	DestroyAttachedAntibodies();
	Destroy();
}

void ABurstVirus::SetNewRandomDirection()
{
	FRotator RandRotator = UKismetMathLibrary::RandomRotator();
	SetActorRotation(FRotator(0.f, RandRotator.Yaw, GetActorRotation().Roll));
}