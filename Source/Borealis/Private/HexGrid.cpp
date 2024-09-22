// Copyright (c) Christian Delicaat 2024. All rights reserved.

#include "../Public/HexGrid.h"

AHexGrid::AHexGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	ProceduralMesh->SetupAttachment(RootComponent);

	const FHexMetrics Metrics;
	const float TileSpacing = 20.f;
	
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			FHexCell HexCell({
				x * (Metrics.Inradius * 2.f + TileSpacing) + (y % 2 * (Metrics.Inradius + TileSpacing / 2)),
				y * (Metrics.Circumradius * (3.f / 2.f) + TileSpacing),
				0.0f
			});
			
			ProceduralMesh->CreateMeshSection(
				y * Width + x,
				HexCell.Vertices,
				HexCell.Triangles,
				{},
				{},
				{},
				{},
				false
			);
		}
	}
}

void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
}
