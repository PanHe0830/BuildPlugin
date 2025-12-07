#include "Build.h"
#include "BuildEdMode.h"
#include "EditorModeRegistry.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "EditorModeManager.h" // 添加此头文件以获得FEditorModeTools的完整类型定义

void FBuildModule::StartupModule()
{
    FEditorModeRegistry::Get().RegisterMode<FBuildEdMode>(
        FBuildEdMode::EM_BuildEdModeId,
        FBuildEdMode::GetDisplayName(), // NSLOCTEXT("BuildEdMode", "DisplayName", "Build Mode"),
        FSlateIcon("EditorStyle","LevelEditor.LandscapeMode"),
        true
    );

    // 注册菜单回调
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FBuildModule::RegisterMenus));
}

void FBuildModule::ShutdownModule()
{
    FEditorModeRegistry::Get().UnregisterMode(FBuildEdMode::EM_BuildEdModeId);
}

void FBuildModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
    FToolMenuSection& Section = Menu->AddSection("BuildModeSection", NSLOCTEXT("BuildEdMode", "MenuSection", "Build Mode"));

    //Section.AddMenuEntry(
    //    "BuildMode",
    //    NSLOCTEXT("BuildEdMode", "BuildMode", "Build Mode"),
    //    NSLOCTEXT("BuildEdMode", "BuildModeTooltip", "Switch to Block Build Mode"),
    //    FSlateIcon(),
    //    FUIAction(FExecuteAction::CreateRaw(this, &FBuildModule::ActivateBuildMode))
    //);
}

void FBuildModule::ActivateBuildMode()
{
    // 激活自定义 Editor Mode
    if (GLevelEditorModeTools().IsModeActive(FBuildEdMode::EM_BuildEdModeId) == false)
    {
        GLevelEditorModeTools().ActivateMode(FBuildEdMode::EM_BuildEdModeId);
    }
}

IMPLEMENT_MODULE(FBuildModule, Build)