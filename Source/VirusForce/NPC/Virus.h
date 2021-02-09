// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Player/VirusForcePawn.h"
#include "GameFramework/Pawn.h"
#include "Virus.generated.h"

UENUM()
enum class EVirusType : uint8
{
	BaseVirus,
	StraightVirus,
	TrackingVirus
};

UCLASS()
class VIRUSFORCE_API AVirus : public APawn
{
	GENERATED_BODY()
private:
	//flag to check if virus has already been added to marked viruses
	bool IsMarked = false;

	FVector VirusScale = FVector();

	UPROPERTY(EditDefaultsOnly, Category = VirusCollision)
	float BounceStrength = 20000.f;

	void SetVirusReady();

public:
	// Sets default values for this pawn's properties
	AVirus();

protected:
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* VirusMeshComponent;

	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(Category = AI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMarkedVirusComponent* MarkedVirusComponent;

	UPROPERTY(Category = VFX, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* BurstParticleEffect;

	UPROPERTY(Category = VirusProperties, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float VirusSpeed = 1000.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SpawnInAnimation();

public:	
	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = MeshSize, EditDefaultsOnly)
	float MeshRadius;

	UPROPERTY(Category = VirusSpawn, BlueprintReadOnly)
	bool IsVirusReady = false;

	UPROPERTY(Category = Movement, BlueprintReadOnly)
	float SpeedModifier = 1.f;

	bool HasStoppedParticleEffect = false;

	UPROPERTY(Category = Antibody, EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AVirusForceProjectile> AcceptedProjectileClass;

	EVirusType VirusType = EVirusType::BaseVirus;
	
	UPROPERTY(Category = Projectile, BlueprintReadOnly)
	TArray<FName> AvailableSocketNames;

	/** Returns VirusMeshComponent subobject **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetVirusMeshComponent() const { return VirusMeshComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetVirusSpeed() const { return VirusSpeed; }

	FORCEINLINE UParticleSystemComponent* GetParticleEffectComponent() const { return BurstParticleEffect; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//called to add self to list of marked viruses, returns true if successful
	bool AddToMarkedViruses();

	//used to determine number of attached antibodies on a virus
	int32 NumOfAttachedAntibodies();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyAttachedAntibodies();
};
