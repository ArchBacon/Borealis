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
	static void WorldToHex(FVector WorldLocation, FHexMetrics Metrics, int& OutQ, int& OutR);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Hex)
	static void MakeS(int Q, int R, int& S);
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category=Hex)
	static void Hash(const int Q, const int R, int& Hash);
};
