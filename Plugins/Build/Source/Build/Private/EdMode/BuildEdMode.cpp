// My Create Class 
#include "BuildTool.h"
#include "ToolKit/BuildEdModeToolKit.h"
#include "EdMode/BuildEdMode.h"
#include "Types/BuildContext.h"
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

PRAGMA_DISABLE_OPTIMIZATION

FBuildEdMode::FBuildEdMode()
{
    
}

FBuildEdMode::~FBuildEdMode()
{
}

void FBuildEdMode::Enter()
{
	FEdMode::Enter();

    BuildTool = MakeShared<FBuildTool>();;

    if (!Toolkit.IsValid())
    {
        Toolkit = MakeShared<FBuildEdModeToolkit>();
        TSharedPtr<FBuildEdModeToolkit> BuildToolkit = StaticCastSharedPtr<FBuildEdModeToolkit>(Toolkit);
        if (BuildToolkit.IsValid())
        {
            BuildToolkit->SetBuildEdMode(this);
        }
        else
        {
			UE_LOG(LogTemp, Error, TEXT("FBuildEdMode::Enter: Failed to cast Toolkit to FBuildEdModeToolkit"));
        }
        Toolkit->Init(Owner->GetToolkitHost());
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

    if (!InViewportClient->IsPerspective()) // 判断是不是预览模式
    {

    }

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

    FBuildClickedContext clickedContext;
    clickedContext.World = World;
    clickedContext.Key = Click.GetKey();
	clickedContext.HitResult = HitResult;

    BuildTool->OnClick(clickedContext);

    return true; // 消费点击事件
}

void FBuildEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
    FEdMode::Render(View, Viewport, PDI);
}

void FBuildEdMode::SetCurrentEdMode(EBuildEditMode InMode)
{
    CurrentMode = InMode;
    switch (InMode)
    {
    case EBuildEditMode::Add:
        UE_LOG(LogTemp, Warning, TEXT("Current BuildEdMode Mode Add : %d"), static_cast<uint8>(CurrentMode));
        break;
    case EBuildEditMode::Remove:
        UE_LOG(LogTemp, Warning, TEXT("Current BuildEdMode Mode Remove : %d"), static_cast<uint8>(CurrentMode));
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Current BuildEdMode Mode default : %d"), static_cast<uint8>(CurrentMode));
        break;
    }
}

void FBuildEdMode::SetBuildAsset(UObject* InObject)
{
    if (!InObject)
    {
        SelectedBuildAsset.Reset();
        return;
    }

    if (InObject->IsA<AActor>() ||
        InObject->IsA<UBlueprint>() ||
        InObject->IsA<UStaticMesh>())
    {
        OnBuildAssetChanged(InObject);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unsupported asset type: %s"), *InObject->GetName());
    }
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

void FBuildEdMode::OnBuildAssetChanged(UObject* InObject)
{
    SelectedBuildAsset = InObject;
}

void FBuildEdMode::UpdateViewportCache(FEditorViewportClient* InViewportClient)
{
    
}

PRAGMA_ENABLE_OPTIMIZATION