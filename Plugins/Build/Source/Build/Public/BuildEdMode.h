#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

#define TRANCE_DISTANCE_LENGTH 100000.0f

class FBuildEdMode : public FEdMode
{
public:
    const static FEditorModeID EM_BuildEdModeId;

public:
    FBuildEdMode();
    virtual ~FBuildEdMode();

    virtual void Enter() override;
    virtual void Exit() override;

    /** 输入鼠标点击事件 */
    virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
    
    /** 绘制工具（可选） */
    virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;

    /** 工具模式名字 */
    const static FText GetDisplayName() { return NSLOCTEXT("BuildEdMode", "DisplayName", "Build Mode"); }

    /** 工具ID */
    virtual bool UsesToolkits() const override { return true; }

private:
    // 判断视口是否发生变化
	bool HasViewParametersChanged(FEditorViewportClient* InViewportClient) const;
    FVector CachedCamLocation;
    FRotator CachedCamRotation;
    FIntPoint CachedViewportSize;
    // 判断视口是否发生变化

private:
    // 功能实现类的指针
    TSharedPtr<class FBuildTool> BuildTool;

    TSharedPtr<FSceneView> CachedView;

	// 定义射线检测的最大距离
    const float TraceDistance = TRANCE_DISTANCE_LENGTH;
};
