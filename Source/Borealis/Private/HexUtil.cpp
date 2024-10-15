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
	
	// x * (Metrics.Inradius * 2.f + Spacing) + (y % 2 * (Metrics.Inradius + Spacing / 2)),
	// y * (Metrics.Circumradius * (3.f / 2.f) + Spacing),

	WorldLocation = FVector(Q, R, 0);
}

void UHexUtil::WorldToHex(
	FVector WorldLocation,
	FHexMetrics Metrics,
	FHexCell& Hex
){
	
}
