// Copyright Adam Farmer 2020


#include "TrackingVirus.h"
#include "GameFramework/FloatingPawnMovement.h"

void ATrackingVirus::BeginPlay()
{
	Super::BeginPlay();

}

void ATrackingVirus::AlterMovementSpeed()
{
	Super::MovementComponent->MaxSpeed *= Super::SpeedModifier;
}