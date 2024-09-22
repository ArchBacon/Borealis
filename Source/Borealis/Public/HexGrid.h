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

USTRUCT(Blueprintable)
struct FHexCell
{
	GENERATED_BODY()

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	FHexMetrics Metrics;

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
};

UCLASS()
class BOREALIS_API AHexGrid : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UProceduralMeshComponent* ProceduralMesh;

	int Width = 6;
	int Height = 6;
	
public:	
	AHexGrid();

protected:
	virtual void BeginPlay() override;
};
