// Copyright Epic Games, Inc. All Rights Reserved.

using Tools.DotNETCommon;
using UnrealBuildTool;

public class UnrealLocator : ModuleRules
{
	public UnrealLocator(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.Add(EngineDirectory + "/Source/Runtime/Launch/Public");
		PublicIncludePaths.Add(EngineDirectory + "/Source/Runtime/Launch/Private"); // Yuck. Required for RequiredProgramMainCPPInclude.h. (Also yuck).

		if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Core", "Projects", "DesktopPlatform", "CoreUObject", "ApplicationCore", "Slate", "SlateCore" });
			PrivateDependencyModuleNames.AddRange(new string[] { "UnixCommonStartup" });
 		}
		else
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Core", "Projects", "DesktopPlatform", "ApplicationCore" });
		}

        PrivateDependencyModuleNames.Add("Json");
    }
}
