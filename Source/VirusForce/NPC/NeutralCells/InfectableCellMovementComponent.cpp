// Copyright Adam Farmer 2020


#include "InfectableCellMovementComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInfectableCellMovementComponent::UInfectableCellMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Amp = FMath::RandRange(AmpMin, AmpMax);

	VerticalPeriod = FMath::RandRange(VerticalPeriodMin, VerticalPeriodMax);

	float AxisOffsetX = FMath::RandRange(AxisOffsetMin, AxisOffsetMax);
	float AxisOffsetY = FMath::RandRange(AxisOffsetMin, AxisOffsetMax);
	SwivelAxisX = UKismetMathLibrary::RotateAngleAxis(XAxis, AxisOffsetX, ZAxis);
	SwivelAxisY = UKismetMathLibrary::RotateAngleAxis(YAxis, AxisOffsetY, ZAxis);

	SwivelPeriodX = FMath::RandRange(SwivelPeriodMin, SwivelPeriodMax);
	SwivelPeriodY = FMath::RandRange(SwivelPeriodMin, SwivelPeriodMax);
}


// Called when the game starts
void UInfectableCellMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInfectableCellMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	VerticalCycle = CalculateVerticalCycle(DeltaTime);
	CurrentSwivelX = CalculateSwivel(DeltaTime, SwivelPeriodX, CurrentSwivelX);
	CurrentSwivelY = CalculateSwivel(DeltaTime, SwivelPeriodY, CurrentSwivelY);
	FVector NewLocation = CalculateActorPosition();
	FRotator NewRotation = CalculateActorRotation();
	GetOwner()->SetActorRelativeLocation(NewLocation);
	GetOwner()->SetActorRelativeRotation(NewRotation);
}

float UInfectableCellMovementComponent::CalculateVerticalCycle(float DeltaTime)
{
	float Degrees = DeltaTime * DegreeConversion;
	float Period = Degrees / VerticalPeriod;
	float CyclePosition = Period + VerticalCycle;
	return FGenericPlatformMath::Fmod(CyclePosition, DegreeConversion);
}

FVector UInfectableCellMovementComponent::CalculateActorPosition()
{
	float SinResultOne = FMath::Sin(VerticalCycle) * Amp;

	FVector ParentActorLocation = GetOwner()->GetActorLocation();

	return ParentActorLocation + FVector(0.f, 0.f, SinResultOne);
}

float UInfectableCellMovementComponent::CalculateSwivel(float DeltaTime, float SwivelPeriod, float CurrentSwivel)
{
	float Degrees = DeltaTime * DegreeConversion;
	float Period = Degrees / SwivelPeriod;
	float CyclePosition = Period + CurrentSwivel;
	return FGenericPlatformMath::Fmod(CyclePosition, DegreeConversion);
}

FRotator UInfectableCellMovementComponent::CalculateActorRotation()
{
	FVector SwivelX = UKismetMathLibrary::RotateAngleAxis(SwivelAxisX, CurrentSwivelX, XAxis);
	FVector SwivelY = UKismetMathLibrary::RotateAngleAxis(SwivelAxisY, CurrentSwivelY, YAxis);

	return UKismetMathLibrary::MakeRotFromXY(SwivelX, SwivelY);
}