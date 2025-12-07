#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FBuildEdModeToolkit : public FModeToolkit
{
public:
    virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

    //virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FEdMode* GetEditorMode() const override;
    virtual TSharedPtr<SWidget> GetInlineContent() const override;
    
private:
    TSharedPtr<class SWidget> BuildUIWidget;

    TSharedPtr<SCompoundWidget> ToolkitWidget;
};
