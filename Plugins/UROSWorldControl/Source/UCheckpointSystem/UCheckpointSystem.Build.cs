// Copyright 201    8, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

using UnrealBuildTool;
using System.IO;

public class UCheckpointSystem : ModuleRules
{
	public UCheckpointSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        PublicDependencyModuleNames.AddRange(
			new string[]
                        {
                            "Core"
                            ,"MeshDescription",
                            "RenderCore",
                            "Json",
                            "UROSBridge"
                        }
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
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
