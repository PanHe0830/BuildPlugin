#include "BuildEdMode.h"
#include "Engine/World.h"
#include "EditorViewportClient.h"
#include "Engine/Selection.h"
#include "Editor.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

#include "BuildEdModeToolKit.h"
#include "EditorModeManager.h"
#include "LevelEditor.h"

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

    if (!Toolkit.IsValid())
    {
        Toolkit = MakeShareable(new FBuildEdModeToolkit);
        Toolkit->Init(Owner->GetToolkitHost());
    }
}

void FBuildEdMode::Exit()
{
    if (Toolkit.IsValid())
    {
        Toolkit.Reset();
    }


    FEdMode::Exit();
}

bool FBuildEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
    if (!InViewportClient)
        return false;

    // 获取鼠标屏幕坐标
    FIntPoint MousePos = Click.GetClickPos();

    // 创建 SceneViewFamily
    FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
        InViewportClient->Viewport,
        InViewportClient->GetWorld()->Scene,
        InViewportClient->EngineShowFlags
    ).SetRealtimeUpdate(true));

    FSceneView* View = InViewportClient->CalcSceneView(&ViewFamily);

    // 屏幕坐标投射为世界射线
    FVector WorldOrigin, WorldDir;
    View->DeprojectFVector2D(FVector2D(MousePos.X, MousePos.Y), WorldOrigin, WorldDir);

    // 射线长度
    const float TraceDistance = 100000.0f;
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
        UWorld* World = InViewportClient->GetWorld();
        if (World)
        {
            // Spawn StaticMeshActor
            AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>(
                HitResult.Location,
                FRotator::ZeroRotator
            );

            if (NewActor && NewActor->GetStaticMeshComponent())
            {
                // 使用 LoadObject 在非 UObject 构造函数中加载资源
                UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(
                    nullptr,
                    TEXT("/Engine/BasicShapes/Cube.Cube")
                );

                if (CubeMesh)
                {
                    NewActor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
                    NewActor->SetActorScale3D(FVector(1.0f));
                    NewActor->GetStaticMeshComponent()->MarkRenderStateDirty();
                }
            }

            // Debug Box
            DrawDebugBox(World, HitResult.Location, FVector(50), FColor::Green, false, 2.0f);
        }
    }

    return true; // 消费点击事件
}

void FBuildEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
    FEdMode::Render(View, Viewport, PDI);
    // 可以在这里画辅助线或高亮选择的方块
}
