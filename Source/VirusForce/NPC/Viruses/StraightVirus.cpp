// Copyright Adam Farmer 2020


#include "StraightVirus.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "../../Arena/Arena.h"
#include "Math/Plane.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

AStraightVirus::AStraightVirus()
{		
	//Super::ShipMeshComponent->
	
}

void AStraightVirus::BeginPlay()
{
	Super::BeginPlay();
	Super::ShipMeshComponent->OnComponentHit.AddDynamic(this, &AStraightVirus::OnHit);
	SetBeginDrawVector(GetActorLocation());
}

void AStraightVirus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStraightVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hey I hit a thing"));
	AArena* Arena = Cast<AArena>(OtherActor);
	if (Arena != nullptr)
	{
		SetEndDrawVector(Hit.Location);
		FVector VectorToMirror = GetVectorToMirror();
		FPlane ReflectionPlane = FindReflectionPlane(Hit.Location, Hit.Normal);

		//DEV ONLY
		DrawDebugSolidPlane(GetWorld(), ReflectionPlane, Hit.Location, FVector2D(500, 500), FColor::Blue, false, 10.f);
		auto NormalVectorToMirror = VectorToMirror.GetSafeNormal();
		FVector ReflectedVector = NormalVectorToMirror.MirrorByPlane(ReflectionPlane);
		FRotator NeededRotation = UKismetMathLibrary::FindLookAtRotation(Hit.Location, ReflectedVector);

		SetActorRotation(NeededRotation);

		//reset for next hit
		SetBeginDrawVector(Hit.Location);
	}
}

FVector AStraightVirus::GetVectorToMirror()
{

	return BeginDrawVector - EndDrawVector;
	//return EndDrawVector - BeginDrawVector;
}

FVector AStraightVirus::SetBeginDrawVector(FVector BeginLocation)
{
	BeginDrawVector = BeginLocation;
	return BeginDrawVector;
}

FVector AStraightVirus::SetEndDrawVector(FVector EndLocation)
{
	EndDrawVector = EndLocation;
	return EndDrawVector;
}

FPlane AStraightVirus::FindReflectionPlane(FVector Location, FVector Normal)
{
	FVector CorrectedNormal = FVector::CrossProduct(Normal, FVector::UpVector);

	//FVector CorrectedNormal = FVector::CrossProduct(FVector::UpVector, Normal);

	return FPlane(Location, CorrectedNormal);
}