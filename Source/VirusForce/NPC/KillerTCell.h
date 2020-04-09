// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KillerTCell.generated.h"

UCLASS()
class VIRUSFORCE_API AKillerTCell : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(Category = AI, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMarkedVirusComponent* MarkedVirusComponent;

	class UScoreManager* ScoreManager;

public:
	// Sets default values for this pawn's properties
	AKillerTCell();

private:

	int32 NumberOfVirusesToConsume;

	UFUNCTION(BlueprintCallable)
	void ConsumeVirus(AActor* ActorToConsume);

	void DestroyVirus(class AVirus* VirusToDestroy);

	void DestroySelfWhenFinishedConsuming();

	void IncreaseScoreForConsumingVirus(AVirus* VirusToDestroy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
