// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "../Virus.h"
#include "BurstVirus.generated.h"

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API ABurstVirus : public AVirus
{
	GENERATED_BODY()
	
	protected:
		UPROPERTY(BlueprintReadOnly)
		class AInfectableCell* CellToInfect;

		virtual void BeginPlay() override;

		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	private:
		void InfectCell();

		void CheckWorldForInfectableCell();

	public:
		void SetInfectableCell(AInfectableCell* InfectableCell);
};

