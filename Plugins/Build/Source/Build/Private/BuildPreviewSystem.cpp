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
    Preview.Destroy();
}