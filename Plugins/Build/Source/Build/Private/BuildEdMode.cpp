// My Create Class 
#include "BuildTool.h"
#include "BuildEdModeToolKit.h"
#include "BuildEdMode.h"
// My Create Class 
// Engine Includes
#include "Engine/World.h"
#include "Engine/Selection.h"
// Engine Includes
// Unreal Includes
#include "EditorViewportClient.h"
#include "Editor.h"
#include "EditorModeManager.h"
#include "LevelEditor.h"
// Unreal Includes
// Other Includes
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
// Other Includes
// Debug Includes
#include "DrawDebugHelpers.h"
// Debug Includes

// 工具ID
const FEditorModeID FBuildEdMode::EM_BuildEdModeId = TEXT("EM_BuildEdMode");

FBuildEdMode::FBuildEdMode()
{
    
}

FBuildEdMode::~FBuildEdMode()
{
}

void FBuildEdMode::Enter()
{
	FEdMode::Enter();

    BuildTool = MakeShareable(new FBuildTool());

    if (!Toolkit.IsValid())
    {
        Toolkit = MakeShareable(new FBuildEdModeToolkit);
        StaticCastSharedPtr<FBuildEdModeToolkit>(Toolkit)->Initialize(Owner->GetToolkitHost(), BuildTool);
    }
}

void FBuildEdMode::Exit()
{
    if (Toolkit.IsValid())
    {
        Toolkit.Reset();
    }

    if (BuildTool.IsValid())
    {
        BuildTool.Reset();
    }

    if (CachedView.IsValid())
    {
       CachedView.Reset();
    }

    FEdMode::Exit();
}

bool FBuildEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
    if (!InViewportClient)
        return false;

    UWorld* World = InViewportClient->GetWorld();
    if (!World)
        return false;

    // 获取鼠标屏幕坐标
    FIntPoint MousePos = Click.GetClickPos();

    if (!CachedView.IsValid() || HasViewParametersChanged(InViewportClient))
    {
        FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
            InViewportClient->Viewport,
            InViewportClient->GetWorld()->Scene,
            InViewportClient->EngineShowFlags
        ).SetRealtimeUpdate(true));
        CachedView = MakeShareable(InViewportClient->CalcSceneView(&ViewFamily));

        CachedCamLocation = InViewportClient->GetViewLocation();
        CachedCamRotation = InViewportClient->GetViewRotation();
        CachedViewportSize = InViewportClient->Viewport->GetSizeXY();
    }

    // 屏幕坐标投射为世界射线
    FVector WorldOrigin, WorldDir;
    CachedView->DeprojectFVector2D(FVector2D(MousePos.X, MousePos.Y), WorldOrigin, WorldDir);
    
    FVector End = WorldOrigin + WorldDir * TraceDistance;

    // 执行射线检测
    FHitResult HitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(BuildEdModeClick), true);
    bool bHit = InViewportClient->GetWorld()->LineTraceSingleByChannel(
        HitResult,
        WorldOrigin,
        End,
        ECC_Visibility,
        Params
    );

    if (bHit)
    {
        if (Click.GetKey() == EKeys::LeftMouseButton)
        {
            BuildTool->OnClick(World, HitResult.Location);
        }
    }

    return true; // 消费点击事件
}

void FBuildEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
    FEdMode::Render(View, Viewport, PDI);
}

bool FBuildEdMode::HasViewParametersChanged(FEditorViewportClient* InViewportClient) const
{
    if (!InViewportClient) return true;

    const FVector CamLocation = InViewportClient->GetViewLocation();
    const FRotator CamRotation = InViewportClient->GetViewRotation();
    const FIntPoint ViewportSize = InViewportClient->Viewport->GetSizeXY();

    return (CamLocation != CachedCamLocation ||
        CamRotation != CachedCamRotation ||
        ViewportSize != CachedViewportSize);
}
