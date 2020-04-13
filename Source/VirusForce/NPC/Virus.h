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

public:
	// Sets default values for this pawn's properties
	AVirus();

protected:
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(Category = AI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMarkedVirusComponent* MarkedVirusComponent;

	UPROPERTY(Category = VirusProperties, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float VirusSpeed = 1000.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = MeshSize, EditDefaultsOnly)
	float MeshRadius;

	EVirusType VirusType = EVirusType::BaseVirus;

	TArray<FName> AvailableSocketNames;

	/** Returns ShipMeshComponent subobject **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE UFloatingPawnMovement* GetMovementComponent() const { return MovementComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetVirusSpeed() const { return VirusSpeed; }

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
