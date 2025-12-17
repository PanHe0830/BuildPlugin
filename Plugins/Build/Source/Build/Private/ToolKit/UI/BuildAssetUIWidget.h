#pragma once

#include "CoreMinimal.h"
#include "Types/BuildModeTypes.h"
#include "Widgets/SCompoundWidget.h"

class SBuildAssetWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SBuildAssetWidget) {}
        SLATE_EVENT(FOnBuildAssetChanged, BuildAssetChange)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    void HandleAssetSelected(const FAssetData& AssetData);
private:
    FOnBuildAssetChanged BuildAssetChange;
};