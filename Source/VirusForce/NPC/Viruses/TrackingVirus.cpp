// Copyright Adam Farmer 2020


#include "TrackingVirus.h"

void ATrackingVirus::BeginPlay()
{
	Super::BeginPlay();

	Super::ShipMeshComponent->OnComponentHit.RemoveDynamic(this, &Super::OnHit);
	Super::ShipMeshComponent->OnComponentHit.AddDynamic(this, &ATrackingVirus::OnHit);
}

void ATrackingVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	//if this hits another virus it should bounce off
	if (Cast<AVirus>(OtherActor) != nullptr)
	{
		FVector ImpulseToApply = Hit.ImpactNormal.GetSafeNormal();

		Super::ShipMeshComponent->AddImpulse(ImpulseToApply * BounceStrength);
		UE_LOG(LogTemp, Warning, TEXT("Adding Impulse"));
	}
}