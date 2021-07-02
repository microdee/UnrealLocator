// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealLocator.h"
#include "RequiredProgramMainCPPInclude.h"
#include "DesktopPlatformModule.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include <iostream>

IMPLEMENT_APPLICATION(UnrealLocator, "UnrealLocator")

bool GetEngineRootDirForProject(const FString& ProjectFileName, FString& OutRootDir)
{
	FString Identifier;
	return FDesktopPlatformModule::Get()->GetEngineIdentifierForProject(ProjectFileName, Identifier) && FDesktopPlatformModule::Get()->GetEngineRootDirFromIdentifier(Identifier, OutRootDir);
}

bool LocateEngine(FOutputDeviceConsole* Output)
{
	if(!Output) return false;
	FString RootDir;

	if(!FDesktopPlatformModule::Get()->GetDefaultEngineRootDir(RootDir))
	{
		return false;
	}
	//Output->Log(RootDir);
	std::wcout << *RootDir << TEXT("\n");

	return !RootDir.IsEmpty();
}

bool LocateEngine(FOutputDeviceConsole* Output, const FString& IdOrProject)
{
	if (!Output) return false;
	FString RootDir;

	bool bIsProject = IdOrProject.EndsWith(TEXT(".uproject"));
	if (bIsProject)
	{
		if(!GetEngineRootDirForProject(IdOrProject, RootDir))
		{
			return false;
		}
	}
	else
	{
		if(!FDesktopPlatformModule::Get()->GetEngineRootDirFromIdentifier(IdOrProject, RootDir))
		{
			return false;
		}
	}
	//Output->Log(RootDir);
	std::wcout << *RootDir << TEXT("\n");
	return !RootDir.IsEmpty();
}

bool EnumerateEngines()
{
	TMap<FString, FString> Installations;
	FDesktopPlatformModule::Get()->EnumerateEngineInstallations(Installations);
	if(Installations.Num() == 0)
		return false;

	for(auto& installation : Installations)
	{
		std::wcout << *installation.Key << TEXT(" | ") << *installation.Value << TEXT("\n");
	}
	
	return true;
}

int Main(const TArray<FString>& Arguments)
{
	bool bRes = false;

	FOutputDeviceConsole* Output = FPlatformApplicationMisc::CreateConsoleOutputDevice();
	Output->Show(true);

	if (Arguments.Num() == 0)
	{
		bRes = EnumerateEngines();
	}
	else
	{
		bRes = LocateEngine(Output, Arguments[0]);
	}
	return bRes ? 0 : 1;
}

#if PLATFORM_WINDOWS

	#include "Windows/AllowWindowsPlatformTypes.h"
	#include <Shellapi.h>

	int WINAPI main(int ArgC, char* ArgV[])
	{
		FCommandLine::Set(TEXT(""));

		TArray<FString> Arguments;
		for (int Idx = 1; Idx < ArgC; Idx++)
		{
			FString Argument = ArgV[Idx];
			if(Argument.Len() > 0 && Argument[0] == '/')
			{
				Argument[0] = '-';
			}
			Arguments.Add(Argument);
		}

		return Main(Arguments);
	}

	#include "Windows/HideWindowsPlatformTypes.h"

#elif PLATFORM_LINUX

	extern TArray<FString> GArguments;

	int32 UnrealLocatorMain( const TCHAR* CommandLine )
	{
		FCommandLine::Set(CommandLine);

		GEngineLoop.PreInit(CommandLine);

		ProcessNewlyLoadedUObjects();

		FModuleManager::Get().StartProcessingNewlyLoadedObjects();

		int32 Result = Main(GArguments);

		FEngineLoop::AppPreExit();
		FModuleManager::Get().UnloadModulesAtShutdown();

	#if STATS
		FThreadStats::StopThread();
	#endif

		FTaskGraphInterface::Shutdown();
		return Result;
	}

#else

	int main(int ArgC, const char* ArgV[])
	{
		FCommandLine::Set(TEXT(""));
		
		TArray<FString> Arguments;
		for (int Idx = 1; Idx < ArgC; Idx++)
		{
			Arguments.Add(ArgV[Idx]);
		}
		
		return Main(Arguments);
	}

#endif
