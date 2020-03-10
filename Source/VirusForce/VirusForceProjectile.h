// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VirusForceProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class AVirusForceProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AVirusForceProjectile();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	//UPROPERTY(Category = Attachment, EditDefaultsOnly, BlueprintReadWrite)
	//class UPhysicsConstraintComponent* PhysicsConstraintComponent;

	bool IsAttached = false;

private:
	void DestroyProjectile();

	FName FindNearestSocketName(class AVirusForcePawn* HitVirus, FHitResult Hit);
};

