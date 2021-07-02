using System;
using System.Linq;
using Nuke.Common;
using Nuke.Common.CI;
using Nuke.Common.Execution;
using Nuke.Common.IO;
using Nuke.Common.ProjectModel;
using Nuke.Common.Tooling;
using Nuke.Common.Utilities.Collections;
using Nuke.Unreal;
using static Nuke.Common.EnvironmentInfo;
using static Nuke.Common.IO.FileSystemTasks;
using static Nuke.Common.IO.PathConstruction;

public class UnrealLocator : IProgramInfo
{
    RelativePath IProgramInfo.PathToProject =>
        (RelativePath) "UnrealLocator.uproject";
}

public class Programs : UnrealProgram
{
    public static Programs UnrealLocator = MakeProgram<UnrealLocator>();

    public static Programs MakeProgram<TProgram>() where TProgram : IProgramInfo =>
        MakeProgram<Programs, TProgram>();
}

[CheckBuildProjectConfigurations]
class Build : ProgramTargets<Programs>
{
    /// Support plugins are available for:
    ///   - JetBrains ReSharper        https://nuke.build/resharper
    ///   - JetBrains Rider            https://nuke.build/rider
    ///   - Microsoft VisualStudio     https://nuke.build/visualstudio
    ///   - Microsoft VSCode           https://nuke.build/vscode

    public static int Main () => Execute<Build>(x => x.BuildEditor);

    [Solution] readonly Solution Solution;

    public override Programs Program { get; set; } = Programs.UnrealLocator;
    public override string UnrealVersion { get; set; } = "4.26";
    public override AbsolutePath CustomEnginePath { get; set; } =
        (AbsolutePath) @"C:\programs\UnrealEngine";
}
