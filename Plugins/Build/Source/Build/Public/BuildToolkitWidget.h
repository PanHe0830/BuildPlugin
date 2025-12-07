#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SBuildToolkitWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SBuildToolkitWidget) {}
        SLATE_ARGUMENT(TSharedPtr<class FBuildTool>, BuildTool)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    void OnResourceSelected(const FAssetData& AssetData);

    TSharedPtr<class FBuildTool> BuildTool;
};