// Copyright (c) Christian Delicaat 2024. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "HexGrid.generated.h"

USTRUCT()
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

USTRUCT()
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

	TArray<FVector> Vertices {};
	TArray<int32> Triangles {};
	FHexMetrics Metrics {};

	UPROPERTY(VisibleAnywhere)
	FLinearColor Color = FLinearColor::White;
	UPROPERTY(VisibleAnywhere)
	FVector Location = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETerrainType> Type = Grassland;

	FHexCell() = default;
	FHexCell(const FVector& Center)
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

	bool operator ==(const FHexCell& Other) const
	{
		return Type == Other.Type;
	}
};

inline uint32_t GetTypeHash(const FHexCell& Hex)
{
	return FCrc::MemCrc32(&Hex, sizeof(FHexCell));
}

// template<>
// struct hash<FHexCell>
// {
// 	size_t operator()(const FHexCell& H) const
// 	{
// 		const std::hash<int32_t> IntHash;
// 		const size_t HQ = IntHash(H.Location.X);
// 		const size_t HR = IntHash(H.Location.X);
// 		return HQ ^ (HR + 0x9e3779b9 + (HQ << 6) + (HQ >> 2));
// 	}
// };

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
	TSet<FHexCell> Map = {};

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
