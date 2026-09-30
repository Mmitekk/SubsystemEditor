#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "SubsystemEditor.generated.h"

/**
 * Base EditorSubsystem class with Blueprint support,
 * lifecycle events, and a dynamic type-safe getter node.
 *
 * ⚠️ IMPORTANT: this file must live in an Editor-only module.
 * Replace YOURPROJECTEDITOR_API with your editor module API macro
 * (module name in ALL_CAPS + _API, e.g. YourProjectEditor -> YOURPROJECTEDITOR_API).
 */
UCLASS(Blueprintable, BlueprintType)
class YOURPROJECTEDITOR_API USubsystemEditor : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem lifecycle overrides
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Blueprint lifecycle events (overrideable in BP under Functions -> Override)
	UFUNCTION(BlueprintImplementableEvent, Category = "Subsystem", meta = (DisplayName = "On Initialize"))
	void ReceiveInitialize();

	UFUNCTION(BlueprintImplementableEvent, Category = "Subsystem", meta = (DisplayName = "On Deinitialize"))
	void ReceiveDeinitialize();

	/** Returns the instance of the specified Editor Subsystem class, automatically casting the output pin. Editor only — returns null in packaged game. */
	UFUNCTION(BlueprintPure, Category = "Subsystems|Editor", meta = (DeterminesOutputType = "SubsystemClass", ToolTip = "Returns the instance of the specified Editor Subsystem class. Editor only."))
	static USubsystemEditor* GetCustomSubsystem(TSubclassOf<USubsystemEditor> SubsystemClass);
};
