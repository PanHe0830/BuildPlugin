#include "BuildTool.h"

#include "DrawDebugHelpers.h"

#include "Engine/StaticMeshActor.h"

FBuildTool::FBuildTool()
{
    ProduceStaticMesh = LoadObject<UStaticMesh>(
        nullptr,
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );
}

FBuildTool::~FBuildTool()
{
}

void FBuildTool::OnClick(UWorld* ViewPortClientWorld, const FVector& ClickLocation)
{
	if (!ViewPortClientWorld)
		return;
	CreateMeshAtLocation(ViewPortClientWorld,ClickLocation);
}

void FBuildTool::CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FVector& Location)
{
    // Spawn StaticMeshActor
    AStaticMeshActor* NewActor = ViewPortClientWorld->SpawnActor<AStaticMeshActor>(
        Location,
        FRotator::ZeroRotator
    );

    if (NewActor && NewActor->GetStaticMeshComponent())
    {
        // 使用 LoadObject 在非 UObject 构造函数中加载资源

        if (ProduceStaticMesh)
        {
            NewActor->GetStaticMeshComponent()->SetStaticMesh(ProduceStaticMesh);
            NewActor->SetActorScale3D(FVector(1.0f));
            NewActor->GetStaticMeshComponent()->MarkRenderStateDirty();
        }
    }
	DrawDebugBox(ViewPortClientWorld, Location, FVector(50), FColor::Green, false, 2.0f);
}
