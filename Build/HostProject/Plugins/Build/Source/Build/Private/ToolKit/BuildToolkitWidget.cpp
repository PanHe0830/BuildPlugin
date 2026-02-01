#include "BuildToolkitWidget.h"
#include "UI/BuildAssetUIWidget.h"
#include "UI/BuildModeUIWidget.h"

void SBuildToolkitWidget::Construct(const FArguments& InArgs)
{
    OnBuildModeChange = InArgs._OnBuildModeChange;
    OnAssetChanged = InArgs._OnAssetChanged;

    ChildSlot
    [
        SNew(SVerticalBox)
            // 模式按钮水平排列
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4)
            [
                SNew(SBuildModeUIWidget)
                    .BuildModeChange(OnBuildModeChange)
                    // 如果你以后加 CurrentMode，可在这里传
            ]

            // 资源选择 UI
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4)
            [
                SNew(SBuildAssetWidget)
                    .BuildAssetChange(OnAssetChanged)
            ]
    ];
}
