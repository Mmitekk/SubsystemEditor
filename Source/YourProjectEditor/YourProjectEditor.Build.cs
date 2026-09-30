// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class YourProjectEditor : ModuleRules
{
	public YourProjectEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Core modules + EditorSubsystem (for UEditorSubsystem).
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EditorSubsystem" });

		// UnrealEd provides GEditor, used by USubsystemEditor::GetCustomSubsystem().
		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
	}
}
