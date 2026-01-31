#pragma once

#include "CoreMinimal.h"
#include "Types/BuildContext.h"

struct FBuildClickedContext;

enum class EPlacementFailReason : uint8
{
	None,
	InvalidAsset,
	BlockedByCollision,
};

struct FPlacementResult
{
	bool bCanPlace = false;
	FTransform FinalTransform;
	EPlacementFailReason FailReason;
	FString FailMessage;
};

class FBuildTool
{
public:
	FBuildTool();
	~FBuildTool();

	void OnClick(const FBuildClickedContext& context);
private:
	void CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FHitResult& Location, UObject* BuildAsset, EBuildAssetType type, TArray<AActor*> IgnoreActors, TArray<UStaticMeshComponent*> IgnoreStaticMeshComponent);

	void DeleteMeshAtLocation(AActor* DeleActor);

public:
	FPlacementResult CanPlaceActorAtLocation(UWorld* ViewPortClientWorld, const FHitResult& HitResult, FBoxSphereBounds BuildBound, EBuildAssetType type, TArray<AActor*> IgnoreActors, TArray<UStaticMeshComponent*> IgnoreStaticMeshComponent);
};
