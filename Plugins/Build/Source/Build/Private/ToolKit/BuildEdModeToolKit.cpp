#include "ToolKit/BuildEdModeToolKit.h"

#include "EdMode/BuildEdMode.h"
#include "EditorModeManager.h"
#include "BuildToolkitWidget.h"
#include "BuildTool.h"

void FBuildEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{// 使用自定义 SCompoundWidget
    ToolkitWidget = SNew(SBuildToolkitWidget)
        .OnAddMode(this, &FBuildEdModeToolkit::OnAddModeSelected)
        .OnDeleteMode(this, &FBuildEdModeToolkit::OnDeleteModeSelected)
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

UObject* FBuildEdModeToolkit::GetSelectedBuildAsset() const
{
    if (SelectedBuildAsset.IsValid())
    {
        return SelectedBuildAsset.Get();
	}
    return nullptr;
}

void FBuildEdModeToolkit::OnBuildAssetChanged(UObject* InObject)
{
    SelectedBuildAsset = InObject;

    if (BuildEdMode != nullptr)
    {
        BuildEdMode->SetBuildAsset(InObject);
    }
}

FReply FBuildEdModeToolkit::OnAddModeSelected()
{
    if (BuildEdMode != nullptr)
    {
        BuildEdMode->SetCurrentEdMode(EBuildEditMode::Add);
    }

    return FReply::Handled();
}

FReply FBuildEdModeToolkit::OnDeleteModeSelected()
{
    if (BuildEdMode != nullptr)
    {
        BuildEdMode->SetCurrentEdMode(EBuildEditMode::Remove);
    }

    return FReply::Handled();
}