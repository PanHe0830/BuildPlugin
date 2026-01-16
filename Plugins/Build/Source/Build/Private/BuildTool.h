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

	// 销毁预览 Actor
	void DestroyPreviewActor();
private:
	// 功能实现
	// 在指定位置创建网格体
	void CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FHitResult& Location, UObject* BuildAsset, EBuildAssetType type);

	// 删除指定位置的物体
	void DeleteMeshAtLocation(AActor* DeleActor);

	// 生成预览 Actor
	void SpawnPreViewActor();

private:
	// 预览用的 Actor
	TWeakObjectPtr<AActor> PreviewActor;
	UStaticMeshComponent* PreviewMeshComponent = nullptr;
};