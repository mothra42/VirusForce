// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Player/VirusForcePawn.h"
#include "GameFramework/Pawn.h"
#include "Virus.generated.h"

UCLASS()
class VIRUSFORCE_API AVirus : public APawn
{
	GENERATED_BODY()
private:
	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(Category = AI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMarkedVirusComponent* MarkedVirusComponent;

	//flag to check if virus has already been added to marked viruses
	bool IsMarked = false;

public:
	// Sets default values for this pawn's properties
	AVirus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = MeshSize, EditDefaultsOnly)
	float MeshRadius;

	TArray<FName> AvailableSocketNames;

	/** Returns ShipMeshComponent subobject **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//called to add self to list of marked viruses, returns true if successful
	bool AddToMarkedViruses();
};
