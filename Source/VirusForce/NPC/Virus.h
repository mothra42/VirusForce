// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../VirusForcePawn.h"
#include "GameFramework/Pawn.h"
#include "Virus.generated.h"

UCLASS()
class VIRUSFORCE_API AVirus : public AVirusForcePawn
{
	GENERATED_BODY()
private:
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//called to add self to list of marked viruses, returns true if successful
	bool AddToMarkedViruses();
};
