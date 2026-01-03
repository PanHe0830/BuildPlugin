#include "BuildTool.h"

#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"


FBuildTool::FBuildTool()
{
    
}

FBuildTool::~FBuildTool()
{
}

void FBuildTool::OnClick(const struct FBuildClickedContext& context)
{
    if (context.Key == EKeys::LeftMouseButton and context.bHit)
    {
		CreateMeshAtLocation(context.World, context.HitResult.Location,context.BuildAsset.Get(),context.AssetType);
    }
}

void FBuildTool::CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FVector& Location, UObject* BuildAsset, EBuildAssetType type)
{
    if (BuildAsset == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation BuildAsset is nullptr"));
		return;
    }
	UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation at Location: %s"), *Location.ToString());

    if (type == EBuildAssetType::Actor)
    {
		AActor* ProduceActor = Cast<AActor>(BuildAsset);
        if (ProduceActor == nullptr)
        {
			UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation BuildAsset is not AActor"));
            return;
        }
        AActor* NewActor = ViewPortClientWorld->SpawnActor<AActor>(
            AActor::StaticClass(),
            Location,
            FRotator::ZeroRotator
		);
        DrawDebugBox(ViewPortClientWorld, Location, FVector(50), FColor::Green, false, 2.0f);
    }
    else if (type == EBuildAssetType::StaticMesh)
    {
        UStaticMesh* ProduceStaticMesh = Cast<UStaticMesh>(BuildAsset);
        if (ProduceStaticMesh == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation BuildAsset is not UStaticMesh"));
            return;
        }
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AStaticMeshActor* MeshActor =
            ViewPortClientWorld->SpawnActor<AStaticMeshActor>(
                Location,
                FRotator::ZeroRotator,
                SpawnParams
            );

        if (!MeshActor)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Failed to spawn AStaticMeshActor"));
            return;
        }

        // 设置 Mesh
        UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
        if (MeshComp)
        {
            MeshComp->SetStaticMesh(ProduceStaticMesh);
            MeshComp->SetMobility(EComponentMobility::Movable);
        }
        DrawDebugBox(ViewPortClientWorld, Location, FVector(50), FColor::Green, false, 2.0f);

		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation EBuildAssetType::StaticMesh"));
    }
    else if (type == EBuildAssetType::BluePrint)
    {
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation EBuildAssetType::BluePrint"));
    }
    else
    {
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation Unknown EBuildAssetType"));
    }
        

#if 0
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
#endif
}
