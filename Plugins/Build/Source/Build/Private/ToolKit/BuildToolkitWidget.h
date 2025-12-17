#pragma once

#include "CoreMinimal.h"
#include "Types/BuildModeTypes.h"
#include "Widgets/SCompoundWidget.h"

class SBuildToolkitWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SBuildToolkitWidget) {}
        SLATE_EVENT(FOnBuildModeChanged, OnBuildModeChange)
        SLATE_EVENT(FOnBuildAssetChanged, OnAssetChanged)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FOnBuildModeChanged OnBuildModeChange;
    FOnBuildAssetChanged OnAssetChanged;
};