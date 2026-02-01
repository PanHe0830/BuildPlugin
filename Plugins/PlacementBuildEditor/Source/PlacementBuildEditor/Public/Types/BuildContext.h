#pragma once

#include "CoreMinimal.h"        // 包含 FVector, FRotator 等基础类型
#include "Engine/World.h"       // UWorld
#include "Engine/EngineTypes.h" // FHitResult
#include "InputCoreTypes.h"     // FKey
#include "BuildModeTypes.h"     // EBuildEditMode

UENUM()
enum class EBuildAssetType : uint8
{
    Actor,
	StaticMesh,
	BluePrint,

	None UMETA(Hidden)
};

struct FBuildClickedContext
{
    FBuildClickedContext()
    {
        World = nullptr;
        bHit = false;
		BuildAsset = nullptr;
        AssetType = EBuildAssetType::None;
        BuildMode = EBuildEditMode::Add;
        IgnoreActors.Empty();
		IgnoreComponents.Empty();
    }
    UWorld* World;
    FHitResult HitResult;
    FKey Key;
    bool bHit;
    TWeakObjectPtr<UObject> BuildAsset;
    EBuildAssetType AssetType;
    EBuildEditMode  BuildMode;
    TArray<AActor*> IgnoreActors;
    TArray<UStaticMeshComponent*> IgnoreComponents;
};