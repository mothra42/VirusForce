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

	if (FoundActors.Num() >= 0)
	{
		SetInfectableCell(Cast<AInfectableCell>(FoundActors[0]));
	}
}

void ABurstVirus::SetInfectableCell(AInfectableCell* InfectableCell)
{
	if (InfectableCell != nullptr)
	{
		CellToInfect = InfectableCell;
	}
}

void ABurstVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//AInfectableCell* CellToInfect = Cast<AInfectableCell>(OtherActor);
	if (Cast<AInfectableCell>(OtherActor) != nullptr)
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