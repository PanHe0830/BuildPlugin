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
	TSharedRef<SWidget> CreateThumbnailWidget();
    TSharedRef<SWidget> CreateAssetPickerWidget();
	TSharedRef<SWidget> CreateAssetTextWidget();
private:
    void HandleAssetSelected(const FAssetData& AssetData);
    FString GetSelectAssetPath() const;
	FText GetAssetText() const;
private:
    FOnBuildAssetChanged BuildAssetChange;
	TWeakObjectPtr<UObject> SelectedDisplayAsset;

	TSharedPtr<FAssetThumbnail> AssetThumbnail;
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool;
    TSharedPtr<SBox> ThumbnailBox;
};