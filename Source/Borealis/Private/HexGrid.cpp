// Copyright (c) Christian Delicaat 2024. All rights reserved.

#include "../Public/HexGrid.h"

#include "HexUtil.h"
#include "Components/TextRenderComponent.h"

AHexGrid::AHexGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	ProceduralMesh->SetupAttachment(RootComponent);

	const FHexMetrics Metrics;
	int Left = 0, Right = 2, Top = 0, Bottom = 2;
	// int Left = 0, Right = 6, Top = 0, Bottom = 4;
	for (int r = Top, i = 0; r <= Bottom; r++)
	{
		int ROffset = floor(r/2.0f);
		for (int q = Left - ROffset; q <= Right - ROffset; q++)
		{
			FVector WorldLocation = FVector::ZeroVector;
			UHexUtil::HexToWorld(FVector(q, r, -q-r), Metrics, WorldLocation);
			
			auto Hex = FHexCell();
			Hex.Location = WorldLocation;
			Hex.Location.Z = 0.0f;
			Hex.Type = (ETerrainType)FMath::RandRange(0, ETerrainType::MAX - 1);
			
			Map.Add(Hex);
			
			UTextRenderComponent* Text = CreateDefaultSubobject<UTextRenderComponent>(*FString::Printf(TEXT("Text Render Component%i"), i++));
			Text->Text = FText::FromString(FString::Printf(TEXT("%i\n%i\n%i"), q, r, -q-r));
			Text->SetRelativeLocation(Hex.Location + FVector(0.0f, 0.0f, 1.0f));
			Text->SetRelativeRotation({90.f, 90.f, 180.f});
			Text->SetHorizontalAlignment(EHTA_Center);
			Text->SetVerticalAlignment(EVRTA_TextCenter);
			Text->SetupAttachment(ProceduralMesh);

			ProceduralMesh->CreateMeshSection_LinearColor(
				r * Width + q,
				Hex.Vertices,
				Hex.Triangles,
				{},
				{},
				{},
				{},
				true
			);
		}
	}
	
	// const FHexMetrics Metrics;
	// for (int y = 0; y < Height; y++)
	// {
	// 	for (int x = 0; x < Width; x++)
	// 	{
	// 		FVector Location = {
	// 			x * (Metrics.Inradius * 2.f + Spacing) + (y % 2 * (Metrics.Inradius + Spacing / 2)),
	// 			y * (Metrics.Circumradius * (3.f / 2.f) + Spacing),
	// 			0.0f
	// 		};
	// 		
	// 		FHexCell HexCell(Location);
	// 		HexCell.Location = Location;
	// 		HexCell.Type = Grassland;
	// 		Cells.Add(HexCell);
	// 		
	// 		UTextRenderComponent* Text = CreateDefaultSubobject<UTextRenderComponent>(*FString::Printf(TEXT("Text Render Component%i"), y * Width + x));
	// 		Text->Text = FText::FromString(FString::Printf(TEXT("%i\n%i\n%i"), x, y, -x-y));
	// 		Text->SetRelativeLocation(Location + FVector(0.0f, 0.0f, 1.0f));
	// 		Text->SetRelativeRotation({90.f, 90.f, 180.f});
	// 		Text->SetHorizontalAlignment(EHTA_Center);
	// 		Text->SetVerticalAlignment(EVRTA_TextCenter);
	// 		Text->SetupAttachment(ProceduralMesh);
	//
	// 		ProceduralMesh->CreateMeshSection_LinearColor( y * Width + x, HexCell.Vertices, HexCell.Triangles, {}, {}, {},{},true);
	// 	}
	// }
}

void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	
	ApplyMaterialsToTerrain();
}

void AHexGrid::ApplyMaterialsToTerrain() const
{
	int Left = 0, Right = 6, Top = 0, Bottom = 4;
	for (int r = Top; r <= Bottom; r++)
	{
		int ROffset = floor(r/2.0f);
		for (int q = Left - ROffset; q <= Right - ROffset; q++)
		{
			// const auto HexCell = Map[FHexCell(FVector(q, r, -q-r))];
			const auto HexCell = *Map.Find(FHexCell(FVector(q, r, -q-r)));
			ProceduralMesh->SetMaterial(r * Width + q, GetTerrainMaterial(HexCell.Type));
		}
	}
	
	// for (int y = 0; y < Height; y++)
	// {
	// 	for (int x = 0; x < Width; x++)
	// 	{
	// 		const auto HexCell = Cells[y * Width + x];
	// 		ProceduralMesh->SetMaterial(y * Width + x, GetTerrainMaterial(HexCell.Type));
	// 	}
	// }
}

UMaterialInterface* AHexGrid::GetTerrainMaterial(const TEnumAsByte<ETerrainType> Type) const
{
	if (!TerrainData)
	{
		return nullptr;
	}
	
	for (const FName Name : TerrainData->GetRowNames())
	{
		if (const auto Row = TerrainData->FindRow<FTerrainTypes>(Name, nullptr))
		{
			if (Row->Type == Type)
			{
				return Row->Material;
			}
		}
	}

	return nullptr;
}
