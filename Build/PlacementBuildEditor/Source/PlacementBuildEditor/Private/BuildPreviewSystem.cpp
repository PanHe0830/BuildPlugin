#include "BuildPreviewSystem.h"

void FBuildPreviewSystem::StartPreview(UWorld* ViewPortClientWorld , UStaticMesh* InMesh)
{
    StopPreview(); // 保证唯一

    if (!InMesh) return;

    FActorSpawnParameters Params;
    Params.ObjectFlags |= RF_Transient;
    Params.bTemporaryEditorActor = true;

    AActor* Actor = ViewPortClientWorld->SpawnActor<AActor>(Params);
    if (!Actor) return;

    UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(Actor);

    MeshComp->SetStaticMesh(InMesh);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MeshComp->SetMobility(EComponentMobility::Movable);
    MeshComp->SetGenerateOverlapEvents(false);
    MeshComp->RegisterComponent();

    Actor->SetRootComponent(MeshComp);

    Preview.Actor = Actor;
    Preview.MeshComponent = MeshComp;
}


void FBuildPreviewSystem::UpdatePreviewTransform(const FTransform& InTransform)
{
    if (!Preview.IsValid()) return;

    Preview.Actor->SetActorTransform(InTransform);
}

void FBuildPreviewSystem::StopPreview()
{
    if (!Preview.IsValid()) return;

    Preview.Destroy();
}

void FBuildPreviewSystem::SetPreviewMaterial(UMaterialInterface* InMaterial)
{
    if (!Preview.IsValid() && Preview.MeshComponent.IsValid()) return;

    if (InMaterial == nullptr)
    {
        for (int32 i = 0; i < Preview.MeshComponent.Get()->GetNumMaterials(); ++i)
        {
            Preview.MeshComponent.Get()->SetMaterial(i, nullptr);
        }
        return;
    }

    UMaterialInstanceDynamic* MaterialDunamic = UMaterialInstanceDynamic::Create(InMaterial, Preview.Actor.Get());
	//Preview.MeshComponent.Get()->SetMaterial(0, nullptr);
    for (int32 i = 0; i < Preview.MeshComponent.Get()->GetNumMaterials(); ++i)
    {
        Preview.MeshComponent.Get()->SetMaterial(i, MaterialDunamic);
    }
}
