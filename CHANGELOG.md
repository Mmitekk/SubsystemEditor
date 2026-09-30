# Changelog

All notable changes to this project will be documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.0.0] - 2026-09-30
### Added
- Initial release: `USubsystemEditor` base class (`SubsystemEditor.h` / `SubsystemEditor.cpp`).
- Blueprintable `UEditorSubsystem` parent for UE 5.0–5.8.
- Blueprint lifecycle events: `On Initialize` (`ReceiveInitialize`), `On Deinitialize` (`ReceiveDeinitialize`).
- Type-safe static getter `GetCustomSubsystem` (`DeterminesOutputType = SubsystemClass`) via `GEditor->GetEditorSubsystemBase()`.
- Full Editor-module template: `Source/YourProjectEditor/` (`YourProjectEditor.Build.cs` with `EditorSubsystem` + `UnrealEd` dependencies, module `.h` / `.cpp` boilerplate, `Public/` + `Private/` layout).
- Bilingual README (EN / RU): why an Editor-only module is required, file map (repo → project), placeholder renaming, `.uproject` registration, Editor Target setup, usage in Editor Utility Blueprints, packaging guidance (exclude editor Blueprints via Directories to Never Cook), optional Asset Manager dedicated nodes.

[1.0.0]: https://github.com/Mmitekk/SubsystemEditor/releases/tag/v1.0.0
