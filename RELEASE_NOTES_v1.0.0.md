# SubsystemEditor v1.0.0 — Initial release

Blueprintable base class for **Editor Subsystems** in Unreal Engine 5 (companion to [SubsystemGameInstance](https://github.com/Mmitekk/SubsystemGameInstance)).

## What's included
- `Source/YourProjectEditor/` — full Editor-module template (just copy + rename placeholders):
  - `YourProjectEditor.Build.cs` — `EditorSubsystem` (public) + `UnrealEd` (private, for `GEditor`)
  - `Public/YourProjectEditor.h` + `Private/YourProjectEditor.cpp` — module boilerplate
  - `Public/SubsystemEditor.h` + `Private/SubsystemEditor.cpp` — `USubsystemEditor : public UEditorSubsystem`
- Blueprint lifecycle events: `On Initialize` / `On Deinitialize` (Functions → Override)
- Getter node `Get Custom Subsystem` with auto-cast (`DeterminesOutputType = SubsystemClass`)
- Bilingual README (EN/RU) + `CHANGELOG.md`

## Requirements
- Unreal Engine 5.0–5.8, C++ project
- Copy the module into `Source/`, rename `YourProjectEditor` → `<YourProject>Editor` and `YOURPROJECTEDITOR_API` → `<YOURPROJECT>EDITOR_API`
- Register the module in `.uproject` (`Type: Editor`) and add it to `YourProjectEditor.Target.cs` (`ExtraModuleNames`)
- Right-click `.uproject` → Generate Visual Studio project files → compile

## How to use (short)
1. Copy + rename the module files, register in `.uproject` + Editor Target, compile.
2. Content Browser → Blueprint Class → parent `SubsystemEditor` (e.g., `BP_SubsystemLevelAudit`).
3. Override `On Initialize` / `On Deinitialize` in the Blueprint.
4. In Editor Utility Widgets/Blueprints call `Get Custom Subsystem` → pick your class → use directly (or use stock `Get Editor Subsystem`).

Full docs: see `README.md`.
