// Copyright (c) Christian Delicaat 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "HexGrid.generated.h"

USTRUCT(BlueprintType)
struct FHexMetrics
{
	GENERATED_BODY()

	float Circumradius = 50.f;
	float Inradius = Circumradius * (FMath::Sqrt(3.0f) / 2); // Circumradius * 0.866025404f

	// Axis conversion: unity ( x, y, z)
	//					unreal(-x, z, y)
	TArray<FVector> Corners = {
		{0.f, Circumradius, 0.f},
		{-Inradius, 0.5f * Circumradius, 0.5},
		{-Inradius, -0.5f * Circumradius, 0.f},
		{0.f, -Circumradius, 0.f},
		{Inradius, -0.5f * Circumradius, 0.f},
		{Inradius, 0.5f * Circumradius, 0.f},
		{0.f, Circumradius, 0.f},
	};
};

UENUM()
enum ETerrainType
{
	Ocean,		// 0
	Coastal,	// 1
	Grassland,	// 2
	MAX,		// 3
};

USTRUCT(BlueprintType)
struct FTerrainTypes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETerrainType> Type = Grassland;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material = nullptr;
};

USTRUCT(Blueprintable)
struct FHexCell
{
	GENERATED_BODY()

	TArray<FVector> Vertices {}; // Not public
	TArray<int32> Triangles {}; // Not public
	FHexMetrics Metrics {};
	
	UPROPERTY(VisibleAnywhere)
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere)
	FIntVector HexCoord = FIntVector::ZeroValue;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETerrainType> Type = Grassland;

	FHexCell() = default;
	FHexCell(const FVector& Center)
		: Location(Center)
	{
		for (int i = 0; i < 6; i++)
		{
			int32 VertexIndex = Vertices.Num();
			
			Vertices.Add(Center);
			Vertices.Add(Center + Metrics.Corners[i]);
			Vertices.Add(Center + Metrics.Corners[i + 1]);
			
			Triangles.Add(VertexIndex);
			Triangles.Add(VertexIndex + 2);
			Triangles.Add(VertexIndex + 1);
		}
	}

	static size_t Hash(const int Q, const int R)
	{
		return HashCombine(Q, R);
	}
};

UCLASS()
class BOREALIS_API AHexGrid : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	UProceduralMeshComponent* ProceduralMesh;

	int Width = 6;
	int Height = 6;
	float Spacing = 0.f;

	UPROPERTY(VisibleAnywhere)
	TArray<FHexCell> Cells = {};

	UPROPERTY(VisibleAnywhere)
	TMap<int, FHexCell> Map = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UDataTable* TerrainData = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UMaterialInterface* ActiveMaterial = nullptr;
	
public:	
	AHexGrid();

protected:
	virtual void BeginPlay() override;

	void ApplyMaterialsToTerrain() const;
	UMaterialInterface* GetTerrainMaterial(TEnumAsByte<ETerrainType> Type) const;
};
