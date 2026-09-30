#include "SubsystemEditor.h"
#include "Editor.h"

void USubsystemEditor::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ReceiveInitialize();
}

void USubsystemEditor::Deinitialize()
{
	ReceiveDeinitialize();
	Super::Deinitialize();
}

USubsystemEditor* USubsystemEditor::GetCustomSubsystem(TSubclassOf<USubsystemEditor> SubsystemClass)
{
	if (!SubsystemClass)
	{
		return nullptr;
	}

	// GEditor exists only in the Editor. In a packaged game or commandlet it can be null.
	if (!GEditor)
	{
		return nullptr;
	}

	return Cast<USubsystemEditor>(GEditor->GetEditorSubsystemBase(SubsystemClass));
}
