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
	class UStaticMeshComponent* CellWallComponent;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CoreComponent;

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

	TArray<AVirus*> RemoveVirusFromMarkedViruses(AVirus* VirusToRemove);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = MarkedViruses, BlueprintReadOnly)
	TArray<AVirus*> MyMarkedViruses;

	FVector ActorLocation = FVector(0.f, 0.f, 0.f);

	FVector* ActorLocationPointer;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FORCEINLINE class UStaticMeshComponent* GetCellWallComponent() const { return CellWallComponent; }

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
