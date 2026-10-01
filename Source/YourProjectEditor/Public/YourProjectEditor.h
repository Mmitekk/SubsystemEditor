// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FYourProjectEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// Deferred activation: runs after the editor finished initializing.
	void ActivateBlueprintEditorSubsystems();
	// Runs once the Asset Registry finished initial discovery, then activates subsystems.
	void ActivateBlueprintEditorSubsystemsNow();

	FDelegateHandle PostEngineInitHandle;
	FDelegateHandle FilesLoadedHandle;
};
