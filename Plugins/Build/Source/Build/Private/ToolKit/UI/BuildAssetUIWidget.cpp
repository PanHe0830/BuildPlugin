#include "BuildAssetUIWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SBuildAssetWidget::Construct(const FArguments& InArgs)
{
    BuildAssetChange = InArgs._BuildAssetChange;
    ChildSlot
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight().Padding(5)
                [
                    SNew(SObjectPropertyEntryBox)
                        .AllowedClass(UObject::StaticClass())
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
