// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VirusForcePawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchProjectileTypeDelegate, int32, ProjectileIndex);

UCLASS(Blueprintable)
class AVirusForcePawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CellWallComponent;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CoreComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(Category = Projectile, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AVirusForceProjectile>> AntibodyTypes;

	UPROPERTY(Category = Debug, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bGodModeOn = false;
	
public:
	AVirusForcePawn();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	UPROPERTY(Category = Projectile, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AVirusForceProjectile> ProjectileClass;

	UPROPERTY(Category = Projectile, BlueprintAssignable)
	FSwitchProjectileTypeDelegate SwitchProjectileTypeDelegate;

	UPROPERTY(Category = SpawnedKillerTCell, EditDefaultsOnly)
	TSubclassOf<class AKillerTCell> KillerTCellClass;

	UPROPERTY(Category = Movement, EditAnywhere, BlueprintReadOnly)
	FVector PlayerVelocity;

	UPROPERTY(Category = VFX, EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* CellDeathEmitter;

	TArray<FName> AvailableSocketNames;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	void LoseLife();

	void SetPlayerInvisible();

	void SetProjectile(TSubclassOf<AVirusForceProjectile> ProjectileToSet);

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;
	static const FName SpawnKillerTCell;

protected:
	virtual void BeginPlay() override;

private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	bool bCanMove = true;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	int32 ProjectileIndexTracker = 0;

	void SpawnKillerTCellInWorld();

	void SwitchAntibodyTypeUp();

	void SwitchAntibodyTypeDown();

public:
	/** Returns CellWallComponent subobject **/
	UFUNCTION()
	FORCEINLINE class UStaticMeshComponent* GetCellWallComponent() const { return CellWallComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE void DisableMovement() { bCanMove = false; }
};

