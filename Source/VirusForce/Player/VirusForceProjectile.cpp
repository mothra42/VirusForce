// Copyright 1998-2019 Epic Games, Inc. All Rights Reserve

#include "VirusForceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "VirusForcePawn.h"
#include "../NPC/Virus.h"

AVirusForceProjectile::AVirusForceProjectile() 
{
	// Static reference to the mesh to use for the projectile
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AVirusForceProjectile::OnHit);		// set up a notification for when this component hits something
	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	//projectiles don't need to expire based on time
	InitialLifeSpan = 0.0f;
}

void AVirusForceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//if the hit actor is a virus attach and set is attached true.
		AVirus* HitVirus = Cast<AVirus>(OtherActor);
		if (HitVirus != nullptr)
		{
			FName NearestSocketName = FindNearestSocketName(HitVirus, Hit);

			if (NearestSocketName != FName("None"))
			{
				ProjectileMovement->StopMovementImmediately();
				ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				ProjectileMesh->AttachToComponent(HitVirus->GetShipMeshComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, NearestSocketName);
				
				//remove the used socket from available names
				HitVirus->AvailableSocketNames.Remove(NearestSocketName);
				//set is attached to true
				IsAttached = true;
				AttachedVirus = HitVirus;
				AddVirusToMarkedViruses(HitVirus);
				return;
			}
			DestroyProjectile();
		}
		else
		{
			DestroyProjectile();
		}
	}
}

FName AVirusForceProjectile::FindNearestSocketName(AVirus* HitVirus, FHitResult Hit)
{
	//attach projectile to available socket
	UStaticMeshComponent* VirusStaticMesh = HitVirus->GetShipMeshComponent();
	//if there are no available slots destroy the projectile
	if (HitVirus->AvailableSocketNames.Num() <= 0)
	{
		DestroyProjectile();
		return FName("None");
	}

	//initialized to an outrageously large number to ensure the logic works
	float SmallestDistanceToSocket = 10000.f;
	FName ClosestSocketName;

	for (int32 i = 0; i < HitVirus->AvailableSocketNames.Num(); i++)
	{
		FVector SocketLocation = VirusStaticMesh->GetSocketLocation(HitVirus->AvailableSocketNames[i]);
		float DistanceToSocket = FMath::Abs(FVector::Distance(SocketLocation, Hit.Location));

		if (DistanceToSocket <= SmallestDistanceToSocket)
		{
			SmallestDistanceToSocket = DistanceToSocket;
			ClosestSocketName = HitVirus->AvailableSocketNames[i];
		}
	}

	return ClosestSocketName;
}

void AVirusForceProjectile::DestroyProjectile()
{
	if (!IsAttached)
	{
		Destroy();
	}
}

void AVirusForceProjectile::AddVirusToMarkedViruses(AVirus* Virus)
{
	if (Virus != nullptr)
	{
		Virus->AddToMarkedViruses();
	}
}

void AVirusForceProjectile::CorrectVelocity(FVector RelativeVelocity)
{
	FVector MyVelocity = ProjectileMovement->Velocity;
	FVector CorrectedVelocity = MyVelocity + RelativeVelocity;
	ProjectileMovement->Velocity = CorrectedVelocity;
}