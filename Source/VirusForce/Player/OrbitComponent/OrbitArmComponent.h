// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "OrbitArmComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIRUSFORCE_API UOrbitArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:

	UOrbitArmComponent();

	UPROPERTY(Category = "Wave Properties", EditDefaultsOnly, BlueprintReadWrite)
	float Cycle;
	
	UPROPERTY(Category = "Wave Properties", EditDefaultsOnly, BlueprintReadWrite)
	float Amplitude;

	UPROPERTY(Category = "Wave Properties", EditDefaultsOnly, BlueprintReadWrite)
	float Period;
};
