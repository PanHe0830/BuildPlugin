#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EBuildEditMode : uint8
{
    Add,
    Remove
};

// 资源选择更改委托
DECLARE_DELEGATE_OneParam(FOnBuildAssetChanged, UObject*);

DECLARE_DELEGATE_OneParam(FOnBuildModeChanged, EBuildEditMode);