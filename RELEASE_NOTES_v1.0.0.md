# SubsystemEditor v1.0.0 — Initial release

Blueprintable base class for **Editor Subsystems** in Unreal Engine 5 (companion to [SubsystemGameInstance](https://github.com/Mmitekk/SubsystemGameInstance)).

## What's included
- `SubsystemEditor.h` / `SubsystemEditor.cpp` — `USubsystemEditor : public UEditorSubsystem`
- Blueprint lifecycle events: `On Initialize` / `On Deinitialize` (Functions → Override)
- Getter node `Get Custom Subsystem` with auto-cast (`DeterminesOutputType = SubsystemClass`)
- Bilingual README (EN/RU) + `CHANGELOG.md`

## Requirements
- Unreal Engine 5.0–5.8, C++ project
- Files must live in an **Editor-only module** (e.g., `Source/YourProjectEditor/`)
- `Build.cs`: add `EditorSubsystem` (public) + `UnrealEd` (private, for `GEditor`)
- Replace `YOUREDITORMODULE_API` with your editor module API macro

## How to use (short)
1. Copy the two files into your editor module, fix the API macro, compile.
2. Content Browser → Blueprint Class → parent `SubsystemEditor` (e.g., `BP_SubsystemLevelAudit`).
3. Override `On Initialize` / `On Deinitialize` in the Blueprint.
4. In Editor Utility Widgets/Blueprints call `Get Custom Subsystem` → pick your class → use directly (or use stock `Get Editor Subsystem`).

Full docs: see `README.md`.
