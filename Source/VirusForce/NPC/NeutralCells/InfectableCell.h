// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfectableCell.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInfectableCellDelegate);

UCLASS()
class VIRUSFORCE_API AInfectableCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInfectableCell();

protected:
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = InfectionProperties)
	TSubclassOf<class AVirus> VirusTypeToSpawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BeginInfection();

	UPROPERTY(Category = InfectionState, BlueprintReadOnly)
	bool InfectedStatus = false;

	UPROPERTY(Category = Infection, BlueprintAssignable)
	FInfectableCellDelegate OnVirusInfection;

	UFUNCTION(BlueprintCallable)
	class USplineComponent* GetNewArtery(UPrimitiveComponent* OverlappedJunctionMesh, USplineComponent* CurrentArtery);

	UFUNCTION(BlueprintCallable)
	float GetTimeOffset(USplineComponent* Artery, UPrimitiveComponent* ArteryJunctionMesh);

private:
	UPROPERTY(EditDefaultsOnly, Category = InfectionProperties)
	float IncubationTime = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = Properties)
	float TimeBeforeAlertViruses = 3.f;

	FTimerHandle TimerHandle_InfectionTimer;

	FTimerHandle TimerHandle_AlertVirusTimer;

	//function called when the infection ends producing more viruses
	void ProduceViruses();

	void AlertVirusesOnSpawn();
};
