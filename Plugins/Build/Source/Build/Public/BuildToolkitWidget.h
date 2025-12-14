#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

// 资源选择更改委托
DECLARE_DELEGATE_OneParam(FOnBuildAssetChanged, UObject*);

class SBuildToolkitWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SBuildToolkitWidget) {}
        SLATE_EVENT(FOnClicked, OnAddMode)
        SLATE_EVENT(FOnClicked, OnDeleteMode)
        SLATE_EVENT(FOnBuildAssetChanged, OnAssetChanged)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    // UI按钮点击处理函数
	// 处理资源选择
    void HandleAssetSelected(const FAssetData& AssetData);
    // 处理添加模式按钮点击
    FReply HandleAddClicked();
    // 处理删除模式按钮点击
    FReply HandleDeleteMode();

private:
    FOnClicked OnAddMode;
    FOnClicked OnDeleteMode;
    FOnBuildAssetChanged OnAssetChanged;
};