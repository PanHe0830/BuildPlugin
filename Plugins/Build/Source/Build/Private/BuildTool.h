#pragma once

#include "CoreMinimal.h"
#include "Types/BuildContext.h"

struct FBuildClickedContext;

class FBuildTool
{
public:
	FBuildTool();
	~FBuildTool();

	void OnClick(const FBuildClickedContext& context);
private:
	// 功能实现
	// 在指定位置创建网格体
	void CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FHitResult& Location, UObject* BuildAsset, EBuildAssetType type, TArray<AActor*> IgnoreActors, TArray<UStaticMeshComponent*> IgnoreStaticMeshComponent);

	// 删除指定位置的物体
	void DeleteMeshAtLocation(AActor* DeleActor);
};