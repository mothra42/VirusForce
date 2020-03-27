// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "../Virus.h"
#include "StraightVirus.generated.h"

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API AStraightVirus : public AVirus
{
	GENERATED_BODY()

	AStraightVirus();
	
protected:
	virtual void BeginPlay() override;

private:
	FVector SetBeginDrawVector(FVector BeginLocation);

	FVector SetEndDrawVector(FVector EndLocation);

	FPlane FindReflectionPlane(FVector Location, FVector Normal);

public:
	FVector BeginDrawVector;
	FVector EndDrawVector;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FVector GetVectorToMirror();
};
