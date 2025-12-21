#pragma once

#include "CoreMinimal.h"        // 包含 FVector, FRotator 等基础类型
#include "Engine/World.h"       // UWorld
#include "Engine/EngineTypes.h" // FHitResult
#include "InputCoreTypes.h"     // FKey
#include "BuildModeTypes.h"     // EBuildEditMode

struct FBuildClickedContext
{
    UWorld* World;
    FHitResult HitResult;
    FKey Key;
    bool bHit;
};