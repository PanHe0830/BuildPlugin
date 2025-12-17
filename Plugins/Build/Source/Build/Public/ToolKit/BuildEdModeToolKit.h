#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FBuildEdModeToolkit : public FModeToolkit
{
public:
    virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;
    void SetBuildEdMode(FBuildEdMode* InBuildEdMode);

    //virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FEdMode* GetEditorMode() const override;
    virtual TSharedPtr<SWidget> GetInlineContent() const override;
    UObject* GetSelectedBuildAsset() const;

private:
	// UI界面按钮点击回调处理函数
    void OnBuildAssetChanged(UObject* InObject);
	// 模式切换回调处理函数
    void OnBuildModeChange(EBuildEditMode BuildMode);
    
private:
    TSharedPtr<class SWidget> BuildUIWidget;

    TSharedPtr<SCompoundWidget> ToolkitWidget;

    TWeakObjectPtr<UObject> SelectedBuildAsset;

    FBuildEdMode* BuildEdMode;
};
