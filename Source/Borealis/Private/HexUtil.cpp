// Copyright (c) Christian Delicaat 2024. All rights reserved.

#include "HexUtil.h"
#include "HexGrid.h"

void UHexUtil::HexToWorld(
	const FHexCell Hex,
	const FHexMetrics Metrics,
	FVector& WorldLocation
){
	const float X = Metrics.Circumradius * (FMath::Sqrt(3.f) * Hex.Location.X + FMath::Sqrt(3.f) / 2 * Hex.Location.Y);
	const float Y = Metrics.Circumradius * (3.f / 2.f * Hex.Location.Y);

	WorldLocation = FVector(X, Y, 0);
}

void UHexUtil::WorldToHex(
	const FVector WorldLocation,
	const FHexMetrics Metrics,
	int& OutQ,
	int& OutR
){
	// Cartesian to Axial
	float Q = (FMath::Sqrt(3.f) / 3.f * WorldLocation.X - 1.0f / 3.f * WorldLocation.Y) / Metrics.Circumradius;
	float R = (2.f / 3.f * WorldLocation.Y) / Metrics.Circumradius;

	// Axial R ound
	int RoundedQ = FMath::RoundToInt(Q);
	int RoundedR = FMath::RoundToInt(R);
	int RoundedS = FMath::RoundToInt(-Q-R);

	float QDiff = FMath::Abs(RoundedQ - Q);
	float RDiff = FMath::Abs(RoundedR - R);
	float SDiff = FMath::Abs(RoundedS - (-Q-R));

	if (QDiff > RDiff && QDiff > SDiff)
	{
		RoundedQ = -RoundedR-RoundedS;
	}
	else if (RDiff > SDiff)
	{
		RoundedR = -RoundedQ-RoundedS;
	}

	OutQ = RoundedQ;
	OutR = RoundedR;
}

void UHexUtil::MakeS(int Q, int R, int& S)
{
	S = -Q-R;	
}

void UHexUtil::Hash(const int Q, const int R, int& Hash)
{
	Hash = FHexCell::Hash(Q, R);
}
