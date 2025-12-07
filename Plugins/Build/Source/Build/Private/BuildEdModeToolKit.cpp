#include "BuildEdModeToolKit.h"

#include "BuildEdMode.h"
#include "EditorModeManager.h"
#include "BuildToolkitWidget.h"

void FBuildEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
    ToolkitWidget = SNew(SBuildToolkitWidget); // 使用自定义 SCompoundWidget
    FModeToolkit::Init(InitToolkitHost);
    BuildUIWidget =
        SNew(SBox)
        .WidthOverride(200)
        .HeightOverride(200)
        [
            ToolkitWidget.ToSharedRef()
        ];
}

//FName FBuildEdModeToolkit::GetToolkitFName() const
//{
//    return FName("BuildEdMode");
//}

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