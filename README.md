# SubsystemEditor for Unreal Engine 5

[English](#english) | [Русский](#русский)

> Blueprintable base class for **Editor Subsystems** + lifecycle events (`On Initialize` / `On Deinitialize`) + type-safe getter node (`Get Custom Subsystem`).
> Companion repo: [SubsystemGameInstance](https://github.com/Mmitekk/SubsystemGameInstance) (Game Instance version).

**Compatibility:** Unreal Engine 5.0 – 5.8 (C++, Editor-only module)

---

<a name="english"></a>
## 🇬🇧 English

`USubsystemEditor` is a C++ base class that enables creating and extending **Editor Subsystems** directly inside Blueprints in Unreal Engine 5. By default, Unreal Engine does not allow creating Blueprint classes directly from `UEditorSubsystem`. This class bridges that gap while adding built-in lifecycle events and a dynamic type-safe getter node — same idea as `USubsystemGameInstance`, but for the Editor.

### ⚠️ IMPORTANT (1): Module API Macro
When adding these files to your project, replace **`YOUREDITORMODULE_API`** in `SubsystemEditor.h` with your own editor module API macro (e.g., `MYGAMEEDITOR_API`, `YOURPROJECTEDITOR_API`). Otherwise the project will fail to compile.

### ⚠️ IMPORTANT (2): Editor-only module
`UEditorSubsystem` works **only in the Unreal Editor** (it does not exist in a packaged game).
- Place these files in an **Editor-only module** (e.g., `Source/YourProjectEditor/`), not in the runtime game module.
- Add the required dependencies to your editor module's `YourProjectEditor.Build.cs`:
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EditorSubsystem" });
PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
```
- `GetCustomSubsystem` returns `null` outside the Editor (packaged game / commandlet without `GEditor`) — this is expected.

### Features
- **Blueprintable**: Inherit your own Blueprint Editor Subsystems (e.g., `BP_SubsystemLevelAudit`, `BP_SubsystemAssetTools`) directly from this class.
- **Lifecycle Events**: Automatically exposes `On Initialize` and `On Deinitialize` events to Blueprints (override via **My Blueprint → Functions → Override**).
- **Custom Getter Node (`Get Custom Subsystem`)**: Static Blueprint Pure node that automatically changes its return pin type based on the selected subsystem class (`DeterminesOutputType`). No manual casting, no broken wires.
- No `WorldContext` needed: Editor subsystems are global to the Editor session, unlike Game Instance subsystems.

### How to Use
1. **Setup C++ files**: Place `SubsystemEditor.h` / `SubsystemEditor.cpp` into `Source/YourProjectEditor/`. Fix the API macro (see above). Add `EditorSubsystem` (+ `UnrealEd` for `GEditor`) to `Build.cs`. Compile.
2. **Create a Blueprint Subsystem**: Content Browser → Right-click → **Blueprint Class** → All Classes → search `SubsystemEditor` → create (e.g., `BP_SubsystemLevelAudit`).
3. **Handle lifecycle**: Open the Blueprint → **My Blueprint → Functions → Override** → `On Initialize` / `On Deinitialize`.
4. **Access in Editor Blueprints** (Editor Utility Widget / Editor Utility Blueprint / Blutility):
   - Right-click → **Get Custom Subsystem** → set **Subsystem Class** to your Blueprint (e.g., `BP_SubsystemLevelAudit`).
   - The output pin auto-casts to that type — call its functions / variables directly.
   - Standard alternative: the built-in **Get Editor Subsystem** node → set Class to your subsystem.
5. **Optional — dedicated node per subsystem** (same trick as in the GameInstance repo): register the Blueprint in **Project Settings → Asset Manager** (Base Class = `SubsystemEditor`, Has Blueprint Classes = true, Directory = your subsystems folder) to get a standalone node like `BP Subsystem Level Audit`. This is editor-time convenience only and is stored in `DefaultGame.ini`.

### API Reference
| Node / Event | Type | Description |
|---|---|---|
| `On Initialize` (`ReceiveInitialize`) | `BlueprintImplementableEvent` | Called from C++ `Initialize()` after the owning editor module is loaded. |
| `On Deinitialize` (`ReceiveDeinitialize`) | `BlueprintImplementableEvent` | Called from C++ `Deinitialize()` before the owning editor module is unloaded. |
| `Get Custom Subsystem` (`GetCustomSubsystem`) | `BlueprintPure`, `DeterminesOutputType = SubsystemClass` | `GEditor->GetEditorSubsystemBase(SubsystemClass)`. Returns `null` if class is null or `GEditor` is missing. |

### Files
- `SubsystemEditor.h` — base class + events + getter declaration.
- `SubsystemEditor.cpp` — `Initialize` / `Deinitialize` forwarding to Blueprint + `GEditor` getter.
- `CHANGELOG.md` — release history.

---

<a name="русский"></a>
## 🇷🇺 Русский

`USubsystemEditor` — базовый C++ класс, который разрешает создание и наследование **Editor Subsystems** прямо в Блюпринтах Unreal Engine 5. По умолчанию движок не даёт наследовать Блюпринты напрямую от `UEditorSubsystem`. Этот класс закрывает пробел, добавляет события жизненного цикла и удобную динамическую ноду получения — та же идея, что и `USubsystemGameInstance`, но для редактора.

### ⚠️ ВАЖНО (1): Макрос API модуля
При добавлении файлов в свой проект замените **`YOUREDITORMODULE_API`** в `SubsystemEditor.h` на макрос API вашего editor-модуля (например, `MYGAMEEDITOR_API`, `YOURPROJECTEDITOR_API`). Без этого проект не скомпилируется.

### ⚠️ ВАЖНО (2): Только Editor-модуль
`UEditorSubsystem` работает **только в редакторе** (в упакованной игре его нет).
- Кладите файлы в **Editor-only модуль** (например, `Source/YourProjectEditor/`), а не в рантайм-модуль игры.
- Добавьте зависимости в `YourProjectEditor.Build.cs`:
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EditorSubsystem" });
PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
```
- Вне редактора (packaged game / commandlet без `GEditor`) `GetCustomSubsystem` вернёт `null` — это нормально.

### Возможности
- **Поддержка Блюпринтов**: создавайте свои Editor-сабсистемы (например, `BP_SubsystemLevelAudit`, `BP_SubsystemAssetTools`), наследуясь от этого класса.
- **События жизненного цикла**: `On Initialize` и `On Deinitialize` доступны через **My Blueprint → Functions → Override**.
- **Кастомная нода (`Get Custom Subsystem`)**: статическая pure-нода, которая сама меняет тип выходного пина под выбранный класс (`DeterminesOutputType`). Никаких кастов и разорванных связей.
- `WorldContext` не нужен: editor-сабсистемы глобальны для сессии редактора (в отличие от Game Instance).

### Как использовать
1. **Файлы**: положите `SubsystemEditor.h` / `SubsystemEditor.cpp` в `Source/YourProjectEditor/`. Поправьте API-макрос (см. выше). Добавьте `EditorSubsystem` (+ `UnrealEd` для `GEditor`) в `Build.cs`. Скомпилируйте.
2. **Блюпринт-сабсистема**: Content Browser → ПКМ → **Blueprint Class** → All Classes → найдите `SubsystemEditor` → создайте (например, `BP_SubsystemLevelAudit`).
3. **Инициализация**: откройте Блюпринт → **My Blueprint → Functions → Override** → `On Initialize` / `On Deinitialize`.
4. **Получение в редакторских Блюпринтах** (Editor Utility Widget / Editor Utility Blueprint / Blutility):
   - Вызовите **Get Custom Subsystem** → в **Subsystem Class** выберите ваш блюпринт.
   - Выходной пин сам примет нужный тип — вызывайте функции и переменные напрямую.
   - Штатная альтернатива: встроенная нода **Get Editor Subsystem** → Class = ваша сабсистема.
5. **Опционально — отдельная нода под каждую сабсистему** (тот же трюк, что и в GameInstance-репозитории): зарегистрируйте блюпринт в **Project Settings → Asset Manager** (Base Class = `SubsystemEditor`, Has Blueprint Classes = true, Directory = папка сабсистем), чтобы получить ноду вида `BP Subsystem Level Audit`. Это удобство уровня редактора, хранится в `DefaultGame.ini`.

### API
| Нода / Ивент | Тип | Описание |
|---|---|---|
| `On Initialize` (`ReceiveInitialize`) | `BlueprintImplementableEvent` | Вызывается из C++ `Initialize()` после загрузки editor-модуля. |
| `On Deinitialize` (`ReceiveDeinitialize`) | `BlueprintImplementableEvent` | Вызывается из C++ `Deinitialize()` перед выгрузкой editor-модуля. |
| `Get Custom Subsystem` (`GetCustomSubsystem`) | `BlueprintPure`, `DeterminesOutputType = SubsystemClass` | `GEditor->GetEditorSubsystemBase(SubsystemClass)`. Возвращает `null`, если класс пуст или нет `GEditor`. |

### Файлы
- `SubsystemEditor.h` — базовый класс + ивенты + декларация геттера.
- `SubsystemEditor.cpp` — проброс `Initialize` / `Deinitialize` в Блюпринт + геттер через `GEditor`.
- `CHANGELOG.md` — история релизов.
