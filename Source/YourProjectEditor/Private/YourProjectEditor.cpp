// Fill out your copyright notice in the Description page of Project Settings.

#include "YourProjectEditor.h"
#include "Modules/ModuleManager.h"
#include "Misc/CoreDelegates.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/Blueprint.h"
#include "Blueprint/BlueprintSupport.h"
#include "Subsystems/SubsystemCollection.h"
#include "SubsystemEditor.h"

void FYourProjectEditorModule::StartupModule()
{
	// The editor subsystem collection does not exist yet at module startup,
	// so activation is deferred until the editor finished initializing.
	PostEngineInitHandle = FCoreDelegates::GetOnPostEngineInit().AddRaw(this, &FYourProjectEditorModule::ActivateBlueprintEditorSubsystems);
}

void FYourProjectEditorModule::ShutdownModule()
{
	FCoreDelegates::GetOnPostEngineInit().Remove(PostEngineInitHandle);
	if (FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
	{
		AssetRegistryModule->Get().OnFilesLoaded().Remove(FilesLoadedHandle);
	}
}

void FYourProjectEditorModule::ActivateBlueprintEditorSubsystems()
{
	// One-shot: never run twice.
	FCoreDelegates::GetOnPostEngineInit().Remove(PostEngineInitHandle);

	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

	// The registry may still be discovering files at this point (background scan).
	// Bind first, then check — this avoids missing the broadcast in a race.
	FilesLoadedHandle = AssetRegistry.OnFilesLoaded().AddRaw(this, &FYourProjectEditorModule::ActivateBlueprintEditorSubsystemsNow);
	if (!AssetRegistry.IsLoadingAssets())
	{
		AssetRegistry.OnFilesLoaded().Remove(FilesLoadedHandle);
		ActivateBlueprintEditorSubsystemsNow();
	}
	// Otherwise ActivateBlueprintEditorSubsystemsNow runs when initial discovery finishes.
}

void FYourProjectEditorModule::ActivateBlueprintEditorSubsystemsNow()
{
	if (FAssetRegistryModule* AssetRegistryModule = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
	{
		AssetRegistryModule->Get().OnFilesLoaded().Remove(FilesLoadedHandle);
	}

	// Blueprint subclasses of UEditorSubsystem are never auto-instanced by the engine
	// (dynamic subsystems are collected from native /Script/* packages only),
	// so we find them via the Asset Registry and register them explicitly.
	FARFilter Filter;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;

	TArray<FAssetData> BlueprintAssets;
	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get().GetAssets(Filter, BlueprintAssets);

	int32 ActivatedCount = 0;
	for (const FAssetData& Asset : BlueprintAssets)
	{
		// NativeParentClass tag holds e.g. "/Script/YourProjectEditor.SubsystemEditor".
		// It matches direct children and grandchildren (BP inheriting from another BP subsystem).
		FString NativeParentClass;
		if (!Asset.GetTagValue(FBlueprintTags::NativeParentClassPath, NativeParentClass) ||
			!NativeParentClass.Contains(TEXT("SubsystemEditor")))
		{
			continue;
		}

		if (const UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset()))
		{
			if (UClass* GeneratedClass = Blueprint->GeneratedClass;
				GeneratedClass && !GeneratedClass->HasAllClassFlags(CLASS_Abstract) &&
				GeneratedClass->IsChildOf(USubsystemEditor::StaticClass()))
			{
				FSubsystemCollectionBase::ActivateExternalSubsystem(GeneratedClass);
				++ActivatedCount;
				UE_LOG(LogTemp, Log, TEXT("YourProjectEditor: activated Editor Subsystem %s"), *GeneratedClass->GetName());
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("YourProjectEditor: activated %d Blueprint Editor Subsystem(s)"), ActivatedCount);
}

IMPLEMENT_MODULE(FYourProjectEditorModule, YourProjectEditor);
