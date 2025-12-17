// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Build : ModuleRules
{
	public Build(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
				"CoreUObject",
				"Engine",
				"InputCore"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
                "EditorSubsystem",
                "EditorFramework",
                "EditorStyle",
                "LevelEditor",
                "UnrealEd",   // <- 必须加上
				"ToolMenus",       // 必须：FToolMenuSection / AddMenuEntry
				"PropertyEditor",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
