// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfectableCellMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRUSFORCE_API UInfectableCellMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInfectableCellMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//used to convert time to degrees for use with sinusoidal function
	float DegreeConversion = 360.f;

	//*** Vertical Movement ***

	//values used to define bound of random Amplitudes
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AmpMax = 10.0;
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AmpMin = 1.0;

	//values used to define bound random Periods
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float VerticalPeriodMax = 200.0;
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float VerticalPeriodMin = 100.0;

	float VerticalCycle = 0.f;

	float Amp;

	float VerticalPeriod;

	float CalculateVerticalCycle(float DeltaTime);

	FVector CalculateActorPosition();

	//*** Swivel Movement ***
	FVector XAxis = FVector(1.0, 0.0, 0.0);
	FVector YAxis = FVector(0.0, 1.0, 0.0);
	FVector ZAxis = FVector(0.0, 0.0, 1.0);

	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AxisOffsetMin = 5.0;
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AxisOffsetMax = 60.0;

	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SwivelPeriodMin = 5.0;
	UPROPERTY(Category = Setup, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SwivelPeriodMax = 6.0;

	float SwivelPeriodX;
	float SwivelPeriodY;

	float CurrentSwivelX;
	float CurrentSwivelY;

	FVector SwivelAxisX;
	FVector SwivelAxisY;

	float CalculateSwivel(float DeltaTime, float SwivelPeriod, float CurrentSwivel);

	FRotator CalculateActorRotation();
};
