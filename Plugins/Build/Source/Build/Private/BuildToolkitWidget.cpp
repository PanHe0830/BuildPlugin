#include "BuildToolkitWidget.h"

// Slate控件头文件
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"
#include "PropertyCustomizationHelpers.h"
// Slate控件头文件

void SBuildToolkitWidget::Construct(const FArguments& InArgs)
{
    OnAddMode = InArgs._OnAddMode;
    OnDeleteMode = InArgs._OnDeleteMode;
    OnAssetChanged = InArgs._OnAssetChanged;

    ChildSlot
    [
        SNew(SVerticalBox)
            // 模式按钮水平排列
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SHorizontalBox)
                    + SHorizontalBox::Slot().AutoWidth().Padding(2.5)
                    [
                        SNew(SButton).Text(FText::FromString("Add Mode"))
                            .OnClicked(this, &SBuildToolkitWidget::HandleAddClicked)
                    ]
                    + SHorizontalBox::Slot().AutoWidth().Padding(2.5)
                    [
                        SNew(SButton).Text(FText::FromString("Delete Mode"))
                            .OnClicked(this, &SBuildToolkitWidget::HandleDeleteMode)
                    ]
            ]
            // 资源选择
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SObjectPropertyEntryBox)
                    .AllowedClass(UObject::StaticClass())
                    .AllowClear(true)
                    .OnObjectChanged(this, &SBuildToolkitWidget::HandleAssetSelected)
            ]
    ];
}

void SBuildToolkitWidget::HandleAssetSelected(const FAssetData& AssetData)
{
    UObject* SelectedObject = AssetData.GetAsset();
    if (OnAssetChanged.IsBound())
    {
        OnAssetChanged.Execute(SelectedObject);
    }
}

FReply SBuildToolkitWidget::HandleAddClicked()
{
    return OnAddMode.IsBound()
        ? OnAddMode.Execute()
        : FReply::Handled();
}

FReply SBuildToolkitWidget::HandleDeleteMode()
{
    return OnDeleteMode.IsBound()
        ? OnDeleteMode.Execute()
        : FReply::Handled();
}
