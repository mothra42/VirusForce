// Copyright Adam Farmer 2020


#include "StraightVirus.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "../../Arena/Arena.h"
#include "Math/Plane.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

AStraightVirus::AStraightVirus()
{		

}

void AStraightVirus::BeginPlay()
{
	Super::BeginPlay();
	Super::VirusMeshComponent->OnComponentHit.RemoveDynamic(this, &Super::OnHit);
	Super::VirusMeshComponent->OnComponentHit.AddDynamic(this, &AStraightVirus::OnHit);
}

void AStraightVirus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AStraightVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	AArena* Arena = Cast<AArena>(OtherActor);
	if (Arena != nullptr)
	{
		FVector ForwardVector = Super::VirusMeshComponent->GetForwardVector() * 500;

		FVector ReflectedVector = ForwardVector.MirrorByVector(Hit.Normal);

		FRotator NeededRotation = UKismetMathLibrary::FindLookAtRotation(Super::VirusMeshComponent->GetForwardVector(), ReflectedVector);

		SetActorRotation(NeededRotation);
	}
}