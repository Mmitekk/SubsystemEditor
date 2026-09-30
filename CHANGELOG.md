# Changelog

All notable changes to this project will be documented in this file.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.0.0] - 2026-09-30
### Added
- Initial release: `USubsystemEditor` base class (`SubsystemEditor.h` / `SubsystemEditor.cpp`).
- Blueprintable `UEditorSubsystem` parent for UE 5.0–5.8.
- Blueprint lifecycle events: `On Initialize` (`ReceiveInitialize`), `On Deinitialize` (`ReceiveDeinitialize`).
- Type-safe static getter `GetCustomSubsystem` (`DeterminesOutputType = SubsystemClass`) via `GEditor->GetEditorSubsystemBase()`.
- Bilingual README (EN / RU): setup in Editor-only module, `Build.cs` dependencies (`EditorSubsystem`, `UnrealEd`), usage in Editor Utility Blueprints, optional Asset Manager dedicated nodes.

[1.0.0]: https://github.com/Mmitekk/SubsystemEditor/releases/tag/v1.0.0
