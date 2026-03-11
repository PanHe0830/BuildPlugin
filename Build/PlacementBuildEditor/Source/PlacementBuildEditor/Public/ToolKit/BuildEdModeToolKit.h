// Copyright (c) 2026 PH

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Types/BuildModeTypes.h"

class FBuildEdMode;

class FBuildEdModeToolkit : public FModeToolkit
{
public:
    virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;
    void SetBuildEdMode(FBuildEdMode* InBuildEdMode);

    //virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FEdMode* GetEditorMode() const override;
    virtual TSharedPtr<SWidget> GetInlineContent() const override;

private:
	// UI界面按钮点击回调处理函数
    void OnBuildAssetChanged(UObject* InObject);
	// 模式切换回调处理函数
    void OnBuildModeChange(EBuildEditMode BuildMode);
private:
    TSharedPtr<class SWidget> BuildUIWidget;

    TSharedPtr<SCompoundWidget> ToolkitWidget;

    FBuildEdMode* BuildEdMode;
};
