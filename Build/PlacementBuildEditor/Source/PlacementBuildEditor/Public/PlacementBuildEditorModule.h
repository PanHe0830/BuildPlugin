// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// 修正包含路径，确保包含正确的头文件
#include "Modules/ModuleManager.h"

class FPlacementBuildEditorModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    /** 注册自定义菜单 */
    void RegisterMenus();

    /** 激活自定义 Editor Mode */
    void ActivateBuildMode();
};
