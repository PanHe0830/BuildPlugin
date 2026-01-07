#include "BuildAssetUIWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "AssetThumbnail.h"       // FAssetThumbnail
#include "Widgets/SBoxPanel.h"   // SBox, SVerticalBox, SHorizontalBox
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"

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

                    // ×ó²à±êÌâ
                    + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(5)
                        [
                            CreateAssetTextWidget()
						]

                    // ×ó²àËõÂÔÍ¼
                    + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(5)
                        [
                            CreateThumbnailWidget()
                        ]

                    // ÓÒ²à×ÊÔ´Ñ¡Ôñ¿ò
                    + SHorizontalBox::Slot()
                        .FillWidth(1.f)
                        [
                            CreateAssetPickerWidget()
                        ]
                ]
        ];
}

TSharedRef<SWidget> SBuildAssetWidget::CreateThumbnailWidget()
{
    return SAssignNew(ThumbnailBox, SBox)
        .WidthOverride(64)
        .HeightOverride(64)
        [
            AssetThumbnail->MakeThumbnailWidget()
        ];
}

TSharedRef<SWidget> SBuildAssetWidget::CreateAssetPickerWidget()
{
    return SNew(SObjectPropertyEntryBox)
        .AllowedClass(UObject::StaticClass())
        .AllowClear(true)
        .OnObjectChanged(this, &SBuildAssetWidget::HandleAssetSelected)
        .ObjectPath(this, &SBuildAssetWidget::GetSelectAssetPath);
}

TSharedRef<SWidget> SBuildAssetWidget::CreateAssetTextWidget()
{
    return SNew(STextBlock)
        .Text(this, &SBuildAssetWidget::GetAssetText);
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

FText SBuildAssetWidget::GetAssetText() const
{
    return SelectedDisplayAsset.IsValid()
        ? FText::FromString(SelectedDisplayAsset->GetName())
        : FText::FromString(TEXT("None"));
}