// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfectableCell.generated.h"

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BeginInfection();

private:
	UPROPERTY(EditDefaultsOnly, Category = InfectionProperties)
	float IncubationTime = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = InfectionProperties)
	TSubclassOf<class AVirus> VirusTypeToSpawn;

	FTimerHandle TimerHandle_InfectionTimer;

	//function called when the infection ends producing more viruses
	void ProduceViruses();

	void AlertVirusesOnSpawn();
};
