// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BaseBallGame : ModuleRules
{
	public BaseBallGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore"
		});
		
		// PublicIncludePaths.Add("BaseBallGame");
	}
}