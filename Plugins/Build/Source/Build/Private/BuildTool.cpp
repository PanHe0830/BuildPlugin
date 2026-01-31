#include "BuildTool.h"

#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"

#include "Editor.h"
#include "Subsystems/EditorActorSubsystem.h"

FBuildTool::FBuildTool()
{
    
}

FBuildTool::~FBuildTool()
{
}

void FBuildTool::OnClick(const struct FBuildClickedContext& context)
{
    switch (context.BuildMode)
    {
        case EBuildEditMode::Add:
            if (context.Key == EKeys::LeftMouseButton and context.bHit)
            {
                CreateMeshAtLocation(context.World, context.HitResult, context.BuildAsset.Get(), context.AssetType,context.IgnoreActors,context.IgnoreComponents);
            }
			break;
		case EBuildEditMode::Remove:
            if (context.Key == EKeys::LeftMouseButton and context.bHit)
            {
                DeleteMeshAtLocation(context.HitResult.GetActor());
            }
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Current Mode is not exist is this BuildTool"));
            break;
    }
}

void FBuildTool::CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FHitResult& HitResult, UObject* BuildAsset, EBuildAssetType type, TArray<AActor*> IgnoreActors, TArray<UStaticMeshComponent*> IgnoreStaticMeshComponent)
{
    if (BuildAsset == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation BuildAsset is nullptr"));
		return;
    }

    if (type == EBuildAssetType::Actor)
    {
        DrawDebugBox(ViewPortClientWorld, HitResult.Location, FVector(50), FColor::Green, false, 2.0f);
    }
    else if (type == EBuildAssetType::StaticMesh)
    {
        UStaticMesh* ProduceStaticMesh = Cast<UStaticMesh>(BuildAsset);
        if (ProduceStaticMesh == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation BuildAsset is not UStaticMesh"));
            return;
        }

		// 拿到 Mesh 的边界信息
        const FBoxSphereBounds MeshBounds = ProduceStaticMesh->GetBounds();
        const FVector HalfExtent = MeshBounds.BoxExtent;

		// 拿到表面法线
        const FVector SurfaceNormal = HitResult.ImpactNormal.GetSafeNormal();

        // 加一个极小偏移，防止浮点精度导致贴边重叠
        constexpr float PlacementEpsilon = 0.1f;

        //const FVector CandidateLocation =
        //    HitResult.ImpactPoint +
        //    SurfaceNormal * (HalfExtent.ProjectOnToNormal(SurfaceNormal).Size() + PlacementEpsilon);

        //拿到被点击物体的 Bounds
        FVector HitHalfExtent = FVector::ZeroVector;

        if (const UPrimitiveComponent* HitComp = HitResult.GetComponent())
        {
            HitHalfExtent = HitComp->Bounds.BoxExtent;
        }

        //沿法线方向分别投影两个 HalfExtent
        const float NewMeshExtentAlongNormal =
            HalfExtent.ProjectOnToNormal(SurfaceNormal).Size();

        const float HitMeshExtentAlongNormal =
            HitHalfExtent.ProjectOnToNormal(SurfaceNormal).Size();

        const FVector CandidateLocation =
            HitResult.ImpactPoint +
            SurfaceNormal * (NewMeshExtentAlongNormal + PlacementEpsilon);

        FCollisionShape CollisionShape =
            FCollisionShape::MakeBox(HalfExtent);

        FCollisionQueryParams QueryParams;
        QueryParams.bTraceComplex = false;
        if (HitResult.GetComponent())
        {
			QueryParams.AddIgnoredComponent(HitResult.GetComponent()); // 忽略被点击组件
            QueryParams.AddIgnoredActor(HitResult.GetActor()); // 忽略被点击物体
        }
        QueryParams.AddIgnoredActors(IgnoreActors);
        for (UStaticMeshComponent* comp : IgnoreStaticMeshComponent)
        {
            if (comp)
            {
                QueryParams.AddIgnoredComponent(comp);  // 忽略每一个组件
            }
        }

        const bool bBlocked =
            ViewPortClientWorld->OverlapBlockingTestByChannel(
                CandidateLocation,
                FQuat::Identity,
                ECC_WorldStatic,
                CollisionShape,
                QueryParams
            );

        if (bBlocked)
        {
            UE_LOG(LogTemp, Warning, TEXT("Placement blocked"));
            return;
        }

        const FScopedTransaction Transaction(
            NSLOCTEXT("BuildTool", "CreateActor", "Create Actor")
        );

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

        AStaticMeshActor* MeshActor =
            ViewPortClientWorld->SpawnActor<AStaticMeshActor>(
                CandidateLocation,
                FRotator::ZeroRotator,
                SpawnParams
            );

        if (!MeshActor)
        {
            UE_LOG(LogTemp, Warning,TEXT("Failed to spawn AStaticMeshActor"));
            return;
        }

        MeshActor->SetFlags(RF_Transactional);
        MeshActor->Modify();

        // 设置 Mesh
        UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
        if (MeshComp)
        {
            MeshComp->SetStaticMesh(ProduceStaticMesh);
            MeshComp->SetMobility(EComponentMobility::Movable);
        }

        DrawDebugBox(ViewPortClientWorld, CandidateLocation, FVector(50), FColor::Green, false, 2.0f);

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
}

void FBuildTool::DeleteMeshAtLocation(AActor* DeleActor)
{
    if (!DeleActor)
    {
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::DeleteMeshAtLocation DeleActor is nullptr"));
        return;
    }

    UEditorActorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
    
    if (!Subsystem)
    {
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::DeleteMeshAtLocation UEditorActorSubsystem is nullptr"));
        return;
    }

    const FScopedTransaction Transaction(
        NSLOCTEXT("BuildTool", "DeleteActor", "Delete Actor")
    );

    DeleActor->Modify();
    Subsystem->DestroyActor(DeleActor);
}

bool FBuildTool::CanPlaceActorAtLocation(FTransform& OutTransform)
{
    return true;
}
