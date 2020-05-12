// Copyright Adam Farmer 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveGameHelper.generated.h"

/**
 * 
 */
UCLASS()
class VIRUSFORCE_API USaveGameHelper : public UObject
{
	GENERATED_BODY()

	//USaveGameHelper();

public:
	void LoadGameDelegate(const FString& SlotName, const int32 UserIndex, class USaveGame* LoadedGameData);
};
