// Copyright ubyte digital. All Right Reserved. https://ubyte.dev

using UnrealBuildTool;

public class WorkerUpstreamNotifyEditor : ModuleRules
{
	public WorkerUpstreamNotifyEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"GameplayTags", 
				"GenericActivityRuntime",
				"WorkerUpstreamNotifyRuntime",
				"GameplayConceptEditor",
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"EditorSubsystem",
				"Projects", 
				"GameplayTagsEditor"
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