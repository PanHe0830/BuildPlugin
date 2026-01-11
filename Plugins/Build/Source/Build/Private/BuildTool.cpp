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
                CreateMeshAtLocation(context.World, context.HitResult.Location, context.BuildAsset.Get(), context.AssetType);
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

        const FScopedTransaction Transaction(
            NSLOCTEXT("BuildTool", "CreateActor", "Create Actor")
        );

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

        MeshActor->SetFlags(RF_Transactional);
        MeshActor->Modify();

        // ÉèÖÃ Mesh
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
