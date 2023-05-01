using UnrealBuildTool;
using System.IO;

public class PCG : ModuleRules
{
    public PCG(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

        // ... add any additional modules or settings as needed
    }
}
