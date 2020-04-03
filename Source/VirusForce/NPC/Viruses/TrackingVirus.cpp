// Copyright Adam Farmer 2020


#include "TrackingVirus.h"
#include "Components/StaticMeshComponent.h"

void ATrackingVirus::BeginPlay()
{
	Super::BeginPlay();

	Super::ShipMeshComponent->OnComponentHit.AddDynamic(this, &ATrackingVirus::OnHit);
}

void ATrackingVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO create a damage system
	UE_LOG(LogTemp, Warning, TEXT("Hey dummy I'm applying damage"));
}