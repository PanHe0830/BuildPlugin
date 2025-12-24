#include "BuildAssetUIWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SNumericEntryBox.h"

PRAGMA_DISABLE_OPTIMIZATION

void SBuildAssetWidget::Construct(const FArguments& InArgs)
{
    BuildAssetChange = InArgs._BuildAssetChange;

    AssetTypes.Add(MakeShareable(new FAssetType(TEXT("Actor"), AActor::StaticClass())));
    AssetTypes.Add(MakeShareable(new FAssetType(TEXT("Blueprint"), UBlueprint::StaticClass())));
    AssetTypes.Add(MakeShareable(new FAssetType(TEXT("StaticMesh"), UStaticMesh::StaticClass())));

    ChildSlot
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight().Padding(5)
                [
                    // 添加类型选择下拉框
                    SNew(SComboBox<TSharedPtr<FAssetType>>)
                        .OptionsSource(&AssetTypes) // 选择项的源
                        .OnGenerateWidget(this, &SBuildAssetWidget::GenerateAssetTypeWidget) // 生成下拉项的处理程序
                        .OnSelectionChanged(this, &SBuildAssetWidget::OnAssetTypeSelected) // 选择变化时的回调
                        .Content()
                        [
                            SNew(STextBlock).Text_Lambda([this]() { return FText::FromString(SelectedAssetType.IsValid() ? SelectedAssetType->Name : TEXT("选择类型")); })
                        ]
                ]
            + SVerticalBox::Slot().AutoHeight().Padding(5)
                [
                    // 初始创建 SObjectPropertyEntryBox，并保存到 ObjectPropertyEntryBox
                    SAssignNew(ObjectPropertyEntryBox, SObjectPropertyEntryBox)
                        .AllowedClass(SelectedAssetType.IsValid() ? SelectedAssetType->Class.Get() : UObject::StaticClass()) // 动态选择类型
                        .AllowClear(true)
                        .OnObjectChanged(this, &SBuildAssetWidget::HandleAssetSelected)
                ]
        ];
}

void SBuildAssetWidget::HandleAssetSelected(const FAssetData& AssetData)
{
    UObject* SelectedObject = AssetData.GetAsset();
    if ( SelectedObject && BuildAssetChange.IsBound() )
    {
        BuildAssetChange.Execute(SelectedObject);
    }
}

// 3. 在选择下拉框类型时，更新 ObjectPropertyEntryBox 的 AllowedClass
void SBuildAssetWidget::OnAssetTypeSelected(TSharedPtr<FAssetType> SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (SelectedItem.IsValid())
    {
        SelectedAssetType = SelectedItem;
        // 在类型选择后更新 SObjectPropertyEntryBox 的 AllowedClass
        RefreshAssetTypeSelection();
    }
}

// 4. 用于刷新 SObjectPropertyEntryBox 筛选条件
void SBuildAssetWidget::RefreshAssetTypeSelection()
{
    if (ObjectPropertyEntryBox.IsValid())
    {
        // 隐藏并销毁当前控件后重建
        ObjectPropertyEntryBox->SetVisibility(EVisibility::Collapsed); // 隐藏当前控件

        // 通过直接清空控件的方式来移除当前对象（如果没有其他对象干扰的话）
        ObjectPropertyEntryBox.Reset(); // 重置控件指针

        // 创建新的控件并设置 AllowedClass
        ChildSlot
            [
                SNew(SObjectPropertyEntryBox)
                    .AllowedClass(SelectedAssetType.IsValid() ? SelectedAssetType->Class.Get() : UObject::StaticClass())
                    .AllowClear(true)
                    .OnObjectChanged(this, &SBuildAssetWidget::HandleAssetSelected)
            ];
    }
}

// 生成下拉项的处理程序
TSharedRef<SWidget> SBuildAssetWidget::GenerateAssetTypeWidget(TSharedPtr<FAssetType> InItem)
{
    return SNew(STextBlock)
        .Text(FText::FromString(InItem.IsValid() ? InItem->Name : TEXT("无效项"))); // 使用每个项的 Name 来显示
}

PRAGMA_ENABLE_OPTIMIZATION