// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "../Virus.h"
#include "TrackingVirus.generated.h"

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API ATrackingVirus : public AVirus
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	EVirusType VirusType = EVirusType::TrackingVirus;

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditDefaultsOnly, Category = VirusCollision);
	float BounceStrength = 500.f;
};
