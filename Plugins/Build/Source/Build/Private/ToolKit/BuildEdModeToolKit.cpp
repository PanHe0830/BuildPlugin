#include "ToolKit/BuildEdModeToolKit.h"

#include "EdMode/BuildEdMode.h"
#include "EditorModeManager.h"
#include "BuildToolkitWidget.h"
#include "BuildTool.h"

void FBuildEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
    // 使用自定义 SCompoundWidget
    ToolkitWidget = SNew(SBuildToolkitWidget)
        .OnBuildModeChange(this, &FBuildEdModeToolkit::OnBuildModeChange)
        .OnAssetChanged(this, &FBuildEdModeToolkit::OnBuildAssetChanged);

    FModeToolkit::Init(InitToolkitHost);
    BuildUIWidget =
        SNew(SBox)
        .WidthOverride(200)
        .HeightOverride(200)
        [
            ToolkitWidget.ToSharedRef()
        ];
}

void FBuildEdModeToolkit::SetBuildEdMode(FBuildEdMode* InBuildEdMode)
{
    BuildEdMode = InBuildEdMode;
}

FText FBuildEdModeToolkit::GetBaseToolkitName() const
{
    return FText::FromString("Build Tool");
}

FEdMode* FBuildEdModeToolkit::GetEditorMode() const
{
    return GLevelEditorModeTools().GetActiveMode(FBuildEdMode::EM_BuildEdModeId);
}

TSharedPtr<SWidget> FBuildEdModeToolkit::GetInlineContent() const
{
    return BuildUIWidget;
}

void FBuildEdModeToolkit::OnBuildAssetChanged(UObject* InObject)
{
    if (BuildEdMode != nullptr)
    {
        BuildEdMode->SetBuildAsset(InObject);
    }
}

void FBuildEdModeToolkit::OnBuildModeChange(EBuildEditMode BuildMode)
{
    if (BuildEdMode != nullptr)
    {
        BuildEdMode->SetCurrentEdMode(BuildMode);
    }
}