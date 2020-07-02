// Copyright Adam Farmer 2020


#include "BurstVirus.h"
#include "../NeutralCells/InfectableCell.h"
#include "Kismet/GameplayStatics.h"

void ABurstVirus::BeginPlay()
{
	Super::BeginPlay();

	Super::ShipMeshComponent->OnComponentHit.RemoveDynamic(this, &Super::OnHit);
	Super::ShipMeshComponent->OnComponentHit.AddDynamic(this, &ABurstVirus::OnHit);
	CheckWorldForInfectableCell();
}

void ABurstVirus::CheckWorldForInfectableCell()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInfectableCell::StaticClass(), FoundActors);

	for (int32 i = 0; i < FoundActors.Num(); i++)
	{
		AInfectableCell* InfectableCell = Cast<AInfectableCell>(FoundActors[i]);
		if (InfectableCell != nullptr && !InfectableCell->InfectedStatus)
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