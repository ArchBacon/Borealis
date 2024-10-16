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
	int Left = -3, Right = 3, Top = -2, Bottom = 2;
	for (int r = Top, i = 0; r <= Bottom; r++)
	{
		int ROffset = floor(r/2.0f);
		for (int q = Left - ROffset; q <= Right - ROffset; q++)
		{
			FVector WorldLocation = FVector::ZeroVector;
			UHexUtil::HexToWorld(FVector(q, r, -q-r), Metrics, WorldLocation);
			
			auto Hex = FHexCell(WorldLocation);
			Hex.HexCoord = FIntVector(q, r, -q-r);
			Hex.Type = (TEnumAsByte<ETerrainType>)FMath::RandRange(0, ETerrainType::MAX - 1);
			Hex.SectionID = i++;
			
			Map.Add(FHexCell::Hash(q, r), Hex);
			
			UTextRenderComponent* Text = CreateDefaultSubobject<UTextRenderComponent>(*FString::Printf(TEXT("Text Render Component%i"), Hex.SectionID));
			Text->Text = FText::FromString(FString::Printf(TEXT("%i\n%i\n%i"), q, r, -q-r));
			Text->SetRelativeLocation(Hex.Location + FVector(0.0f, 0.0f, 1.0f));
			Text->SetRelativeRotation({90.f, 90.f, 180.f});
			Text->SetHorizontalAlignment(EHTA_Center);
			Text->SetVerticalAlignment(EVRTA_TextCenter);
			Text->SetupAttachment(ProceduralMesh);

			ProceduralMesh->CreateMeshSection(
				Hex.SectionID,
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
}

void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	
	ApplyMaterialsToTerrain();
}

void AHexGrid::ApplyMaterialsToTerrain() const
{
	int Left = -3, Right = 3, Top = -2, Bottom = 2;
	for (int r = Top, i = 0; r <= Bottom; r++)
	{
		int ROffset = floor(r/2.0f);
		for (int q = Left - ROffset; q <= Right - ROffset; q++)
		{
			auto Result = Map.Find(FHexCell::Hash(q, r));
			if (!Result)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Cell [%i, %i] not found."), q, r));
				continue;
			}
			
			const auto HexCell = *Result;
			ProceduralMesh->SetMaterial(HexCell.SectionID, GetTerrainMaterial(HexCell.Type));
		}
	}
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
