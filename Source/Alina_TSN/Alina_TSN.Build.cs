// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Alina_TSN : ModuleRules
{
	public Alina_TSN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
