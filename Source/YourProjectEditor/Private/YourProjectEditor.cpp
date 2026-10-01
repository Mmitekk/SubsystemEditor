// Fill out your copyright notice in the Description page of Project Settings.

#include "YourProjectEditor.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/Blueprint.h"
#include "SubsystemEditor.h"

void FYourProjectEditorModule::StartupModule()
{
	// Automatically find and load all Blueprint Editor Subsystems at editor startup
	// so that the engine's subsystem collection instantiates them properly.
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	if (AssetRegistry.IsLoadingAssets())
	{
		AssetRegistry.WaitForCompletion();
	}

	FARFilter Filter;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	for (const FAssetData& Asset : AssetList)
	{
		// Check if the Blueprint inherits from USubsystemEditor by checking its NativeParentClass tag
		FString ParentClassStr = Asset.GetTagValueRef<FString>(FBlueprintTags::NativeParentClass);
		if (ParentClassStr.Contains(TEXT("SubsystemEditor")))
		{
			// Load the asset into memory
			Asset.GetAsset();
		}
	}
}

void FYourProjectEditorModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FYourProjectEditorModule, YourProjectEditor);
