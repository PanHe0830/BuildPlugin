#pragma once

#include "CoreMinimal.h"
#include "Types/BuildModeTypes.h"
#include "Widgets/SCompoundWidget.h"

class SObjectPropertyEntryBox;

struct FAssetType
{
    FString Name;
    TSubclassOf<UObject> Class;

    FAssetType(const FString& InName, TSubclassOf<UObject> InClass)
        : Name(InName), Class(InClass)
    {
    }
};

class SBuildAssetWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SBuildAssetWidget) {}
        SLATE_EVENT(FOnBuildAssetChanged, BuildAssetChange)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    void HandleAssetSelected(const FAssetData& AssetData);
    void OnAssetTypeSelected(TSharedPtr<FAssetType> SelectedItem, ESelectInfo::Type SelectInfo);
    void RefreshAssetTypeSelection();
    TSharedRef<SWidget> GenerateAssetTypeWidget(TSharedPtr<FAssetType> InItem);
private:
    FOnBuildAssetChanged BuildAssetChange;
    TSharedPtr<FAssetType> SelectedAssetType;
    TSharedPtr<SObjectPropertyEntryBox> ObjectPropertyEntryBox;
    TArray<TSharedPtr<FAssetType>> AssetTypes;
};