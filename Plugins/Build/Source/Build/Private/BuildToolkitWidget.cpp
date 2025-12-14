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
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SObjectPropertyEntryBox)
                    .AllowedClass(UObject::StaticClass()) // 可以改成你想允许的类型
                    .AllowClear(true)
                    .OnObjectChanged(this, &SBuildToolkitWidget::HandleAssetSelected)
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(2)
            [
                SNew(SButton).Text(FText::FromString("Delete Mode"))
                    .OnClicked(this,&SBuildToolkitWidget::HandleDeleteMode)
			] 
            + SVerticalBox::Slot().AutoHeight().Padding(2)
            [
                SNew(SButton).Text(FText::FromString("Add Mode"))
                    .OnClicked(this, &SBuildToolkitWidget::HandleAddClicked)
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
