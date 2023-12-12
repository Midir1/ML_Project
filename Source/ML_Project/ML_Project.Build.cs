using UnrealBuildTool;

public class ML_Project : ModuleRules
{
	public ML_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore", "Json", 
			"JsonUtilities"
		});
	}
}
