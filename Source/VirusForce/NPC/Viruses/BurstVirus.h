// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "StraightVirus.h"
#include "BurstVirus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBurstVirusDelegate, AInfectableCell*, CellToTrack);

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API ABurstVirus : public AStraightVirus
{
	GENERATED_BODY()
	
	protected:
		UPROPERTY(BlueprintReadOnly)
		class AInfectableCell* CellToInfect;

		UPROPERTY(Category = TargetedCell, BlueprintAssignable)
		FBurstVirusDelegate OnCellToInfectChanged;

		virtual void BeginPlay() override;

		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	private:
		void InfectCell();

		UFUNCTION()
		void CheckWorldForInfectableCell();

		void SetNewRandomDirection();

	public:
		void SetInfectableCell(AInfectableCell* InfectableCell);
};

