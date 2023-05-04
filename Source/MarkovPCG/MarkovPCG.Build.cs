using UnrealBuildTool; 

public class MarkovPCG: ModuleRules 
{ 
    public MarkovPCG(ReadOnlyTargetRules Target) : base(Target) 
    {       
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"}); 
    } 
}