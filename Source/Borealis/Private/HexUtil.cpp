// Copyright (c) Christian Delicaat 2024. All rights reserved.

#include "HexUtil.h"
#include "HexGrid.h"

void UHexUtil::HexToWorld(
	FHexCell Hex,
	FHexMetrics Metrics,
	FVector& WorldLocation
){
	float Q = Metrics.Circumradius * (FMath::Sqrt(3.f) * Hex.Location.X + FMath::Sqrt(3.f) / 2 * Hex.Location.Y);
	float R = Metrics.Circumradius * (3.f / 2.f * Hex.Location.Y);

	WorldLocation = FVector(Q, R, -Q-R);

	FString Text = FString::Printf("(%f, %f, %f) -> (%f, %f, %f)", Hex.Location.X, Hex.Location.Y, Hex.Location.Z, WorldLocation.X, WorldLocation.Y, WorldLocation.Z);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, Text);
}

void UHexUtil::WorldToHex(
	FVector WorldLocation,
	FHexMetrics Metrics,
	FHexCell& Hex
){
	
}
