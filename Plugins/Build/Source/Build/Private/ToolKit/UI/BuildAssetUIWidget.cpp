#include "BuildAssetUIWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "AssetThumbnail.h"       // FAssetThumbnail
#include "Widgets/SBoxPanel.h"   // SBox, SVerticalBox, SHorizontalBox
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"

PRAGMA_DISABLE_OPTIMIZATION

void SBuildAssetWidget::Construct(const FArguments& InArgs)
{
    BuildAssetChange = InArgs._BuildAssetChange;
    ThumbnailPool = MakeShared<FAssetThumbnailPool>(32);
    AssetThumbnail = MakeShared<FAssetThumbnail>((UObject*)nullptr, 64, 64, ThumbnailPool);
    ChildSlot
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
                [
                    SNew(SHorizontalBox)

                    // ×ó²àËõÂÔÍ¼
                    + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(5)
                        [
                            SAssignNew(ThumbnailBox, SBox)
                                .WidthOverride(64)
                                .HeightOverride(64)
                                [
                                    AssetThumbnail->MakeThumbnailWidget()
                                ]
                        ]

                    // ÓÒ²à×ÊÔ´Ñ¡Ôñ¿ò
                    + SHorizontalBox::Slot()
                        .FillWidth(1.f)
                        [
                            SNew(SObjectPropertyEntryBox)
                                .AllowedClass(UObject::StaticClass())
                                .AllowClear(true)
                                .OnObjectChanged(this, &SBuildAssetWidget::HandleAssetSelected)
                                .ObjectPath(this, &SBuildAssetWidget::GetSelectAssetPath)
                        ]
                ]
        ];
}

void SBuildAssetWidget::HandleAssetSelected(const FAssetData& AssetData)
{
    UObject* SelectedObject = AssetData.GetAsset();
    if ( SelectedObject && BuildAssetChange.IsBound() )
    {
        if (!ThumbnailPool.IsValid())
        {
			ThumbnailPool = MakeShared<FAssetThumbnailPool>(32);
        }
		AssetThumbnail = MakeShared<FAssetThumbnail>(AssetData, 64, 64, ThumbnailPool);
        ThumbnailBox->SetContent(AssetThumbnail->MakeThumbnailWidget());
        SelectedDisplayAsset = SelectedObject;
        BuildAssetChange.Execute(SelectedObject);
        // ¸æËß Slate Ë¢ÐÂ
        //ChildSlot.GetWidget()->Invalidate(EInvalidateWidget::LayoutAndVolatility);
    }
    else
    {
        AssetThumbnail.Reset();
		SelectedDisplayAsset = nullptr;
        ThumbnailBox->SetContent(SNullWidget::NullWidget);// Çå¿ÕÏÔÊ¾
    }
}

FString SBuildAssetWidget::GetSelectAssetPath() const
{
	return SelectedDisplayAsset.IsValid() ? 
        SelectedDisplayAsset->GetPathName() : FString("None Asset");
}

PRAGMA_ENABLE_OPTIMIZATION