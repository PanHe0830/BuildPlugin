#pragma once

#include "CoreMinimal.h"
#include "Types/BuildContext.h"

struct FBuildClickedContext;

enum class EPlacementFailReason : uint8
{
	None,
	InvalidAsset,
	NoHitResult,
	BlockedByCollision,
	UnsupportedSurface,
};

struct FPlacementResult
{
	bool bCanPlace = false; // 是否可以放置 只有当 bCanPlace 为 true 时，FinalTransform 才有效
	FTransform FinalTransform; // 最终用于放置的世界 Transform
	EPlacementFailReason FailReason = EPlacementFailReason::None; // 放置失败的原因
};

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

	/**
	* 判断当前预览 Actor 是否可以在指定规则下放置。
	*
	* 若返回 true，OutTransform 将包含最终用于放置的世界 Transform；
	* 若返回 false，OutTransform 的内容不应被使用。
	*
	* 该判断通常包含：
	* - 放置位置是否命中可建造区域
	* - 是否与已有 Actor 发生碰撞（忽略预览 Actor 本身）
	*
	* @param OutTransform 当放置合法时，返回计算后的放置 Transform
	* @return true 表示满足所有放置规则，可以放置
	*/
	bool CanPlaceActorAtLocation(FTransform& OutTransform);
};