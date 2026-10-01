# Changelog

All notable changes to this project will be documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.0.1] - 2026-10-01
### Fixed
- Blueprint Editor Subsystems never auto-instanced by the engine (`UEditorSubsystem` is a *dynamic* subsystem: only native `/Script/*` classes are collected). The editor module now discovers Blueprint subsystems via the Asset Registry and registers them with `FSubsystemCollectionBase::ActivateExternalSubsystem`, so `Initialize()` / `On Initialize` reliably fire. Activation is deferred in two stages: `FCoreDelegates::GetOnPostEngineInit\(\)` (subsystem collection must exist) then `IAssetRegistry::OnFilesLoaded` (background discovery must finish).
- `YourProjectEditor.Build.cs`: added `AssetRegistry` to private dependencies.
- README (EN / RU): documented why activation is mandatory for Editor (but not Game Instance) subsystems.

## [1.0.0] - 2026-09-30
### Added
- Initial release: `USubsystemEditor` base class (`SubsystemEditor.h` / `SubsystemEditor.cpp`).
- Blueprintable `UEditorSubsystem` parent for UE 5.0–5.8.
- Blueprint lifecycle events: `On Initialize` (`ReceiveInitialize`), `On Deinitialize` (`ReceiveDeinitialize`).
- Type-safe static getter `GetCustomSubsystem` (`DeterminesOutputType = SubsystemClass`) via `GEditor->GetEditorSubsystemBase()`.
- Full Editor-module template: `Source/YourProjectEditor/` (`YourProjectEditor.Build.cs` with `EditorSubsystem` + `UnrealEd` dependencies, module `.h` / `.cpp` boilerplate, `Public/` + `Private/` layout).
- Bilingual README (EN / RU): why an Editor-only module is required, file map (repo → project), placeholder renaming, `.uproject` registration, Editor Target setup, usage in Editor Utility Blueprints, packaging guidance (exclude editor Blueprints via Directories to Never Cook), optional Asset Manager dedicated nodes.

[1.0.1]: https://github.com/Mmitekk/SubsystemEditor/releases/tag/v1.0.1
[1.0.0]: https://github.com/Mmitekk/SubsystemEditor/releases/tag/v1.0.0
