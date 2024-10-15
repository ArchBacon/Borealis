// Copyright (c) Christian Delicaat 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexUtil.generated.h"

struct FHexCell;
struct FHexMetrics;

UCLASS()
class BOREALIS_API UHexUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Hex)
	static void HexToWorld(FHexCell Hex, FHexMetrics Metrics, FVector& WorldLocation);

	UFUNCTION(BlueprintCallable, Category=Hex)
	static void WorldToHex(FVector WorldLocation, FHexMetrics Metrics, FHexCell& Hex);
};
