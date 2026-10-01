# SubsystemEditor for Unreal Engine 5

[English](#english) | [Русский](#русский)

> Blueprintable base class for **Editor Subsystems** + lifecycle events (`On Initialize` / `On Deinitialize`) + type-safe getter node (`Get Custom Subsystem`).
> Companion repo: [SubsystemGameInstance](https://github.com/Mmitekk/SubsystemGameInstance) (Game Instance version).

**Compatibility:** Unreal Engine 5.0 – 5.8 (C++, Editor-only module)

---

<a name="english"></a>
## 🇬🇧 English

`USubsystemEditor` is a C++ base class that enables creating and extending **Editor Subsystems** directly inside Blueprints in Unreal Engine 5. By default, Unreal Engine does not allow creating Blueprint classes directly from `UEditorSubsystem`. This class bridges that gap while adding built-in lifecycle events and a dynamic type-safe getter node — same idea as `USubsystemGameInstance`, but for the Editor.

### ⚠️ IMPORTANT: why an Editor-only module is required
`UEditorSubsystem` exists **only in the Unreal Editor** — it is stripped out of packaged games. If you put `USubsystemEditor` into your runtime game module (e.g. `Source/YourProject/`), packaging will fail or the code will be dead weight in the shipping build. The correct place is a separate module of type `Editor` (e.g. `Source/YourProjectEditor/`), which the engine loads only in the Editor.

This repository already contains **all module files** — you just copy them into your project, rename the placeholders to your project name, and register the module. No need to write `Build.cs` or module boilerplate by hand.

### Files in this repo → where they go in your project
| File in this repo | Copy to (in your project) |
|---|---|
| `Source/YourProjectEditor/YourProjectEditor.Build.cs` | `Source/<YourProject>Editor/<YourProject>Editor.Build.cs` |
| `Source/YourProjectEditor/Public/YourProjectEditor.h` | `Source/<YourProject>Editor/Public/<YourProject>Editor.h` |
| `Source/YourProjectEditor/Private/YourProjectEditor.cpp` | `Source/<YourProject>Editor/Private/<YourProject>Editor.cpp` |
| `Source/YourProjectEditor/Public/SubsystemEditor.h` | `Source/<YourProject>Editor/Public/SubsystemEditor.h` |
| `Source/YourProjectEditor/Private/SubsystemEditor.cpp` | `Source/<YourProject>Editor/Private/SubsystemEditor.cpp` |

Example: if your project is called `KingdomOfIsrion`, the module folder becomes `Source/KingdomOfIsrionEditor/`.

### Setup step by step
**Step 1 — Copy the module.** Copy the whole `Source/YourProjectEditor/` folder from this repo into your project's `Source/` folder.

**Step 2 — Rename placeholders to your project name.** Three things must be renamed consistently (example for `KingdomOfIsrion`):
1. Folder and file names: `YourProjectEditor` → `KingdomOfIsrionEditor` (folder, `.Build.cs`, module `.h` / `.cpp`).
2. Inside `KingdomOfIsrionEditor.Build.cs`: `public class YourProjectEditor` → `public class KingdomOfIsrionEditor`, and the constructor name likewise.
3. Inside module `.h` / `.cpp`: `#include "YourProjectEditor.h"` → `#include "KingdomOfIsrionEditor.h"`, and `IMPLEMENT_MODULE(FDefaultModuleImpl, YourProjectEditor)` → `IMPLEMENT_MODULE(FDefaultModuleImpl, KingdomOfIsrionEditor)`.
4. Inside `SubsystemEditor.h`: `YOURPROJECTEDITOR_API` → `KINGDOMOFISRIONEDITOR_API` (rule: module name in ALL_CAPS + `_API`).

The `SubsystemEditor.h` / `.cpp` file names and the `USubsystemEditor` class name stay as they are.

**Step 3 — Dependencies (already in the .Build.cs).** The provided `YourProjectEditor.Build.cs` already declares everything needed — no edits required:
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EditorSubsystem" });
PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
```
(`EditorSubsystem` gives you `UEditorSubsystem`; `UnrealEd` gives you `GEditor` used by the getter.)

**Step 4 — Register the module in your `.uproject`.** Open `YourProject.uproject` and add the Editor module to the `Modules` array (keep the existing runtime module entry):
```json
"Modules": [
	{
		"Name": "YourProject",
		"Type": "Runtime",
		"LoadingPhase": "Default",
		"AdditionalDependencies": [ "Engine" ]
	},
	{
		"Name": "YourProjectEditor",
		"Type": "Editor",
		"LoadingPhase": "Default"
	}
]
```

**Step 5 — Add the module to the Editor target.** Open `Source/YourProjectEditor.Target.cs` (every C++ project already has one) and extend `ExtraModuleNames`:
```csharp
ExtraModuleNames.AddRange( new string[] { "YourProject", "YourProjectEditor" } );
```
(Do **not** add the Editor module to the game `YourProject.Target.cs` — it stays Editor-only.)

**Step 6 — Generate and compile.** Right-click your `.uproject` → **Generate Visual Studio project files**, then compile (IDE or Editor). If you named everything consistently, the build succeeds and `USubsystemEditor` appears in the Editor's Blueprint class picker.

### Features
- **Blueprintable**: Inherit your own Blueprint Editor Subsystems (e.g., `BP_SubsystemLevelAudit`, `BP_SubsystemAssetTools`) directly from this class.
- **Lifecycle Events**: Automatically exposes `On Initialize` and `On Deinitialize` events to Blueprints (override via **My Blueprint → Functions → Override**).
- **Custom Getter Node (`Get Custom Subsystem`)**: Static Blueprint Pure node that automatically changes its return pin type based on the selected subsystem class (`DeterminesOutputType`). No manual casting, no broken wires.
- **Auto-loading on Startup**: The editor module automatically scans and loads Blueprint editor subsystems at startup via Asset Registry, ensuring `Initialize()` fires reliably.
- No `WorldContext` needed: Editor subsystems are global to the Editor session, unlike Game Instance subsystems.

### How to use (after setup)
1. **Create a Blueprint Subsystem**: Content Browser → Right-click → **Blueprint Class** → All Classes → search `SubsystemEditor` → create (e.g., `BP_SubsystemLevelAudit`). Put it in a dedicated folder, e.g. `Content/Blueprints/Editor/Subsystems/` (see Packaging below — this folder must be excluded from cooking).
2. **Handle lifecycle**: Open the Blueprint → **My Blueprint → Functions → Override** → `On Initialize` / `On Deinitialize`.
3. **Access in Editor Blueprints** (Editor Utility Widget / Editor Utility Blueprint / Blutility):
   - Right-click → **Get Custom Subsystem** → set **Subsystem Class** to your Blueprint.
   - The output pin auto-casts to that type — call its functions / variables directly.
   - Standard alternative: the built-in **Get Editor Subsystem** node → set Class to your subsystem.
4. **Optional — dedicated node per subsystem** (same trick as in the GameInstance repo): register the Blueprint in **Project Settings → Asset Manager** (Base Class = `SubsystemEditor`, Has Blueprint Classes = true, Directory = your subsystems folder) to get a standalone node. Editor-time convenience only, stored in `DefaultGame.ini`.

### Packaging (important)
- **C++ side — nothing to do.** Modules with `"Type": "Editor"` in the `.uproject` are simply not compiled for game targets (`WindowsNoEditor`, etc.), so the Editor module never ends up in a packaged build.
- **Blueprint side — exclusion is required.** The cooker has no editor-only classes, so any cooked asset that references `SubsystemEditor` will fail the cook. To avoid this:
  1. Keep all Editor-subsystem Blueprints in a dedicated folder, e.g. `Content/Blueprints/Editor/`.
  2. Open **Project Settings → Packaging → Directories to Never Cook** and add that folder.
- **Where Blueprints live:** always somewhere under `Content/` (`.uasset` files cannot exist elsewhere). The `/All/C++ Classes/...` view in the Content Browser only *displays* code classes — you cannot (and must not) place Blueprints there; create them in any `Content/` folder you like.

### API Reference
| Node / Event | Type | Description |
|---|---|---|
| `On Initialize` (`ReceiveInitialize`) | `BlueprintImplementableEvent` | Called from C++ `Initialize()` after the owning editor module is loaded. |
| `On Deinitialize` (`ReceiveDeinitialize`) | `BlueprintImplementableEvent` | Called from C++ `Deinitialize()` before the owning editor module is unloaded. |
| `Get Custom Subsystem` (`GetCustomSubsystem`) | `BlueprintPure`, `DeterminesOutputType = SubsystemClass` | `GEditor->GetEditorSubsystemBase(SubsystemClass)`. Returns `null` if class is null or `GEditor` is missing (packaged game / commandlet). |

---

<a name="русский"></a>
## 🇷🇺 Русский

`USubsystemEditor` — базовый C++ класс, который разрешает создание и наследование **Editor Subsystems** прямо в Блюпринтах Unreal Engine 5. По умолчанию движок не даёт наследовать Блюпринты напрямую от `UEditorSubsystem`. Этот класс закрывает пробел, добавляет события жизненного цикла и удобную динамическую ноду получения — та же идея, что и `USubsystemGameInstance`, но для редактора.

### ⚠️ ВАЖНО: зачем нужен отдельный Editor-модуль
`UEditorSubsystem` существует **только в редакторе** — в упакованную игру он не попадает. Если положить `USubsystemEditor` в рантайм-модуль игры (например, `Source/YourProject/`), упаковка проекта сломается или код будет мёртвым грузом в shipping-сборке. Правильное место — отдельный модуль с типом `Editor` (например, `Source/YourProjectEditor/`), который движок грузит только в редакторе.

В этом репозитории уже лежат **все файлы модуля** — их нужно просто скопировать в свой проект, переименовать плейсхолдеры под имя проекта и зарегистрировать модуль. Писать `Build.cs` и обвязку модуля вручную не нужно.

### Файлы репозитория → куда класть в своём проекте
| Файл в репозитории | Куда копировать (в вашем проекте) |
|---|---|
| `Source/YourProjectEditor/YourProjectEditor.Build.cs` | `Source/<ВашПроект>Editor/<ВашПроект>Editor.Build.cs` |
| `Source/YourProjectEditor/Public/YourProjectEditor.h` | `Source/<ВашПроект>Editor/Public/<ВашПроект>Editor.h` |
| `Source/YourProjectEditor/Private/YourProjectEditor.cpp` | `Source/<ВашПроект>Editor/Private/<ВашПроект>Editor.cpp` |
| `Source/YourProjectEditor/Public/SubsystemEditor.h` | `Source/<ВашПроект>Editor/Public/SubsystemEditor.h` |
| `Source/YourProjectEditor/Private/SubsystemEditor.cpp` | `Source/<ВашПроект>Editor/Private/SubsystemEditor.cpp` |

Пример: если проект называется `KingdomOfIsrion`, папка модуля станет `Source/KingdomOfIsrionEditor/`.

### Установка по шагам
**Шаг 1 — Скопируйте модуль.** Скопируйте целиком папку `Source/YourProjectEditor/` из репозитория в папку `Source/` вашего проекта.

**Шаг 2 — Переименуйте плейсхолдеры под имя проекта.** Три вещи переименовываются согласованно (пример для `KingdomOfIsrion`):
1. Имена папки и файлов: `YourProjectEditor` → `KingdomOfIsrionEditor` (папка, `.Build.cs`, модульные `.h` / `.cpp`).
2. Внутри `KingdomOfIsrionEditor.Build.cs`: `public class YourProjectEditor` → `public class KingdomOfIsrionEditor`, имя конструктора — аналогично.
3. Внутри модульных `.h` / `.cpp`: `#include "YourProjectEditor.h"` → `#include "KingdomOfIsrionEditor.h"`, и `IMPLEMENT_MODULE(FDefaultModuleImpl, YourProjectEditor)` → `IMPLEMENT_MODULE(FDefaultModuleImpl, KingdomOfIsrionEditor)`.
4. Внутри `SubsystemEditor.h`: `YOURPROJECTEDITOR_API` → `KINGDOMOFISRIONEDITOR_API` (правило: имя модуля КАПСОМ + `_API`).

Имена файлов `SubsystemEditor.h` / `.cpp` и класса `USubsystemEditor` не меняются.

**Шаг 3 — Зависимости (уже прописаны в .Build.cs).** Приложенный `YourProjectEditor.Build.cs` уже содержит всё нужное, править ничего не надо:
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EditorSubsystem" });
PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
```
(`EditorSubsystem` даёт `UEditorSubsystem`; `UnrealEd` даёт `GEditor` для геттера.)

**Шаг 4 — Зарегистрируйте модуль в `.uproject`.** Откройте `YourProject.uproject` и добавьте Editor-модуль в массив `Modules` (существующую запись рантайм-модуля сохраните):
```json
"Modules": [
	{
		"Name": "YourProject",
		"Type": "Runtime",
		"LoadingPhase": "Default",
		"AdditionalDependencies": [ "Engine" ]
	},
	{
		"Name": "YourProjectEditor",
		"Type": "Editor",
		"LoadingPhase": "Default"
	}
]
```

**Шаг 5 — Добавьте модуль в Editor-Target.** Откройте `Source/YourProjectEditor.Target.cs` (он уже есть в каждом C++ проекте) и расширьте `ExtraModuleNames`:
```csharp
ExtraModuleNames.AddRange( new string[] { "YourProject", "YourProjectEditor" } );
```
(В игровой `YourProject.Target.cs` Editor-модуль **не** добавляйте — он остаётся только для редактора.)

**Шаг 6 — Сгенерируйте и скомпилируйте.** ПКМ по `.uproject` → **Generate Visual Studio project files**, затем компиляция (IDE или редактор). Если всё переименовано согласованно, сборка пройдёт и `USubsystemEditor` появится в выборе родительского Blueprint-класса.

### Возможности
- **Поддержка Блюпринтов**: создавайте свои Editor-сабсистемы (например, `BP_SubsystemLevelAudit`, `BP_SubsystemAssetTools`), наследуясь от этого класса.
- **События жизненного цикла**: `On Initialize` и `On Deinitialize` через **My Blueprint → Functions → Override**.
- **Кастомная нода (`Get Custom Subsystem`)**: статическая pure-нода, сама меняет тип выходного пина под выбранный класс (`DeterminesOutputType`). Никаких кастов и разорванных связей.
- **Автозагрузка при старте**: модуль редактора автоматически сканирует и подгружает Блюпринт-сабсистемы через Asset Registry, гарантируя вызов `Initialize()`.
- `WorldContext` не нужен: editor-сабсистемы глобальны для сессии редактора (в отличие от Game Instance).

### Как использовать (после установки)
1. **Блюпринт-сабсистема**: Content Browser → ПКМ → **Blueprint Class** → All Classes → найдите `SubsystemEditor` → создайте (например, `BP_SubsystemLevelAudit`). Кладите в отдельную папку, например `Content/Blueprints/Editor/Subsystems/` (см. раздел про упаковку ниже — её нужно исключить из кука).
2. **Инициализация**: откройте Блюпринт → **My Blueprint → Functions → Override** → `On Initialize` / `On Deinitialize`.
3. **Получение в редакторских Блюпринтах** (Editor Utility Widget / Editor Utility Blueprint / Blutility):
   - Вызовите **Get Custom Subsystem** → в **Subsystem Class** выберите ваш блюпринт.
   - Выходной пин сам примет нужный тип — вызывайте функции и переменные напрямую.
   - Штатная альтернатива: встроенная нода **Get Editor Subsystem** → Class = ваша сабсистема.
4. **Опционально — отдельная нода под каждую сабсистему** (тот же трюк, что и в GameInstance-репозитории): зарегистрируйте блюпринт в **Project Settings → Asset Manager** (Base Class = `SubsystemEditor`, Has Blueprint Classes = true, Directory = папка сабсистем). Это удобство уровня редактора, хранится в `DefaultGame.ini`.

### Упаковка проекта (важно)
- **C++ сторона — делать ничего не надо.** Модули с `"Type": "Editor"` в `.uproject` просто не компилируются для игровых таргетов (`WindowsNoEditor` и т.п.), поэтому Editor-модуль никогда не попадает в упакованную сборку.
- **Блюпринты — исключение обязательно.** У кукера нет editor-only классов, поэтому любой запекаемый ассет, ссылающийся на `SubsystemEditor`, уронит упаковку с ошибкой. Чтобы этого не было:
  1. Держите все блюпринты Editor-сабсистем в отдельной папку, например `Content/Blueprints/Editor/`.
  2. Откройте **Project Settings → Packaging → Directories to Never Cook** и добавьте туда эту папку.
- **Где живут блюпринты:** всегда где-то внутри `Content/` (`.uasset` файлы больше нигде существовать не могут). Вкладка `/All/C++ Classes/...` в Content Browser только *показывает* классы кода — класть туда блюпринты нельзя (да движок и не даст); создавайте их в любой папке `Content/` на ваш вкус.

### API
| Нода / Ивент | Тип | Описание |
|---|---|---|
| `On Initialize` (`ReceiveInitialize`) | `BlueprintImplementableEvent` | Вызывается из C++ `Initialize()` после загрузки editor-модуля. |
| `On Deinitialize` (`ReceiveDeinitialize`) | `BlueprintImplementableEvent` | Вызывается из C++ `Deinitialize()` перед выгрузкой editor-модуля. |
| `Get Custom Subsystem` (`GetCustomSubsystem`) | `BlueprintPure`, `DeterminesOutputType = SubsystemClass` | `GEditor->GetEditorSubsystemBase(SubsystemClass)`. Возвращает `null`, если класс пуст или нет `GEditor` (упакованная игра / commandlet). |
