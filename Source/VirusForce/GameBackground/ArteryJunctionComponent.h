// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ArteryJunctionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class VIRUSFORCE_API UArteryJunctionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArteryJunctionComponent();

	TArray<class USplineComponent*> ArteryArray;

	TMap<USplineComponent*, int32> ArteryLocationMap;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	class UStaticMeshComponent* CollisionMesh;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetupConnection(USplineComponent* SplineComponent, int32 SplinePoint, FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetCollisionMesh();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetupArteryArrayAndMap(USplineComponent* SplineComponent, int32 SplinePoint);
};
