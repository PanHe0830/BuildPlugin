#include "EdMode/BuildEdMode.h"
#include "ToolKit/BuildEdModeToolKit.h"
#include "Build/Private/BuildPreviewSystem.h"
#include "BuildTool.h"

#include "Engine/World.h"
#include "Engine/Selection.h"

#include "EditorViewportClient.h"
#include "Editor.h"
#include "EditorModeManager.h"
#include "LevelEditor.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

#include "DrawDebugHelpers.h"


// 工具ID
const FEditorModeID FBuildEdMode::EM_BuildEdModeId = TEXT("EM_BuildEdMode");

FBuildEdMode::FBuildEdMode()
{
    BuildPreview = nullptr;
}

FBuildEdMode::~FBuildEdMode()
{

}

void FBuildEdMode::Enter()
{
	FEdMode::Enter();

    BuildTool = MakeShared<FBuildTool>();
	BuildPreview = new FBuildPreviewSystem();

    PreviewMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Build/Material/M_Preview_Failed.M_Preview_Failed"));

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

    if (BuildPreview)
    {
        BuildPreview->StopPreview();
		delete BuildPreview;
		BuildPreview = nullptr;
    }

    FEdMode::Exit();
}

bool FBuildEdMode::MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
    if (!BuildPreview) return false;

    if (CurrentMode != EBuildEditMode::Add)
    {
        return false;
    }

    UStaticMesh* TempMesh = Cast<UStaticMesh>(SelectedBuildAsset.Get());
	if (!TempMesh) return false;

	BuildPreview->StartPreview(ViewportClient->GetWorld(), TempMesh);
    return true;
}

bool FBuildEdMode::MouseLeave(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
    if (!BuildPreview) return false;

	BuildPreview->StopPreview();
    return true;
}

bool FBuildEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
    if (BuildPreview && !BuildPreview->HasValidPreview()) return false;

    if (CurrentMode != EBuildEditMode::Add)
    {
        return false;
    }

    UWorld* World = ViewportClient->GetWorld();
    if (!World) return false;

    UStaticMesh* TempMesh = Cast<UStaticMesh>(SelectedBuildAsset.Get());
    if (!TempMesh) return false;

    FVector CamLocation = ViewportClient->GetViewLocation(); // 获得摄像机在三维世界的坐标
    FVector CamForward = ViewportClient->GetCursorWorldLocationFromMousePos().GetDirection();

    FVector RayOrigin = CamLocation;
    FVector RayDirection = CamForward;
    FVector End = RayOrigin + RayDirection * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(BuildPreviewTrace), true);
	Params.AddIgnoredActor(BuildPreview->GetPreviewActor());
    Params.AddIgnoredComponent(BuildPreview->GetPreviewMeshComponent());
    bool bHit = World->LineTraceSingleByChannel(
        Hit, 
        RayOrigin, 
        End, 
        ECC_Visibility, 
        Params
    );

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(BuildPreview->GetPreviewActor());
    IgnoreActors.Add(Hit.GetActor());

	TArray<UStaticMeshComponent*> IgnoreComponents;
	IgnoreComponents.Add(BuildPreview->GetPreviewMeshComponent());

    FPlacementResult result = BuildTool->CanPlaceActorAtLocation(World, Hit, BuildPreview->GetPreviewBounds(), Type, IgnoreActors, IgnoreComponents);
    if (!result.bCanPlace)
    {
        BuildPreview->SetPreviewMaterial(PreviewMaterial);
    }
    else
    {
        BuildPreview->SetPreviewMaterial(nullptr);
    }
    BuildPreview->UpdatePreviewTransform(result.FinalTransform);

    return true;
}

bool FBuildEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
    if (!InViewportClient || !InViewportClient->IsPerspective() || !BuildTool)
        return false;

    UWorld* World = InViewportClient->GetWorld();
    if (!World)
        return false;

    FVector RayOrigin = Click.GetOrigin();
    FVector RayDirection = Click.GetDirection();
    const FVector End = RayOrigin + RayDirection * TraceDistance;
    // 执行射线检测
    FHitResult HitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(BuildEdModeClick), true);
    Params.AddIgnoredActor(BuildPreview->GetPreviewActor());
    Params.AddIgnoredComponent(BuildPreview->GetPreviewMeshComponent());
    bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        RayOrigin,
        End,
        ECC_Visibility,
        Params
    );

    FBuildClickedContext clickedContext;
    clickedContext.World = World;
    clickedContext.Key = Click.GetKey();
	clickedContext.HitResult = HitResult;
    clickedContext.bHit = bHit;
	clickedContext.AssetType = Type;
	clickedContext.BuildAsset = SelectedBuildAsset.Get();
	clickedContext.BuildMode = CurrentMode;
	clickedContext.IgnoreActors.Add(BuildPreview->GetPreviewActor());
    clickedContext.IgnoreComponents.Add(BuildPreview->GetPreviewMeshComponent());

    BuildTool->OnClick(clickedContext);

    return true;
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
	UE_LOG(LogTemp, Warning, TEXT("SetBuildAsset: %s"), InObject ? *InObject->GetName() : TEXT("None"));
    if (!InObject)
    {
		UE_LOG(LogTemp, Warning, TEXT("Clearing selected build asset."));
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

void FBuildEdMode::OnBuildAssetChanged(UObject* InObject)
{
    SelectedBuildAsset = InObject;
	//BuildPreview->StartPreview(,InObject);
    if ( Cast<AActor>(InObject) )
    {
        Type = EBuildAssetType::Actor;
    }
    else if ( Cast<UBlueprint>(InObject) )
    {
        Type = EBuildAssetType::BluePrint;
    }
    else if ( Cast<UStaticMesh>(InObject) )
    {
        Type = EBuildAssetType::StaticMesh;
    }
    else
    {
        Type = EBuildAssetType::None;
	}
	UE_LOG(LogTemp, Warning, TEXT("Selected build asset changed to: %s, Type: %d"), *InObject->GetName(), static_cast<uint8>(Type));
}