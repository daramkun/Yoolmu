# ECS / World

## 기본 정책

초기 ECS는 sparse set component pool 방식으로 구현한다.

```text
Initial:
  Sparse set ECS

Later:
  Hot component SoA storage
  Archetype ECS 검토
```

전체 ECS를 처음부터 archetype으로 구현하지 않는다.

## Entity

```cpp
struct Entity
{
    uint32_t index;
    uint32_t generation;
};
```

## Component

초기 컴포넌트:

- TransformComponent
- MeshRendererComponent
- CameraComponent
- LightComponent
- AudioSourceComponent
- PhysicsBodyComponent, later
- UiComponent, later

## GameObject Facade

ECS가 실제 저장소이고, GameObject는 편의 API다.

```cpp
class GameObject
{
public:
    template<class T, class... Args>
    T& AddComponent(Args&&... args);

    template<class T>
    T& GetComponent();

private:
    World* world_;
    Entity entity_;
};
```

## RenderExtract

Renderer는 GameObject를 직접 참조하지 않는다.

```text
ECS World
 → RenderExtract
 → RenderWorld / GPU Scene
 → RenderGraph
```

---

# Scene Format

## 초기

초기에는 자체 텍스트 씬 포맷을 사용한다.

예시:

```text
scene "TestScene"

entity "Camera"
  transform 0 2 -5  0 0 0  1 1 1
  camera 60 0.1 1000

entity "Cube"
  transform 0 0 0  0 0 0  1 1 1
  mesh "cube.meshbin"
  material "default.matbin"
```

## Editor / Tools

에디터와 애셋 파이프라인이 안정된 뒤 YAML로 전환한다.

```text
YAML:
  Tools/Editor only

Runtime:
  YAML 직접 로딩 금지
```

`yaml-cpp`는 Tools/Editor only로 허용한다.

## Runtime

Runtime은 최종적으로 cooked scene binary만 읽는다.

```text
scene.yaml
 → SceneCooker
 → .scenebin
 → Runtime
```

---

# UI 정책

## 기본 정책

Dear ImGui는 사용하지 않는다.

자체 UI 시스템을 구현한다.

최종 목표는 Editor UI와 Runtime UI를 하나의 UI 시스템으로 처리하는 것이다.

## 구현 전략

초기에는 debug UI 스타일 API부터 만든다.

```cpp
ui.BeginPanel("Debug");
ui.Label("Frame Time");
ui.SliderFloat("Exposure", &exposure, 0.0f, 4.0f);
ui.Button("Reload Shader");
ui.EndPanel();
```

내부 구조는 retained UI tree / layout / paint command / GPU renderer로 발전시킨다.

```text
Immediate-like debug API
 → UI node cache
 → layout
 → paint command
 → GPU UI renderer
```

## Text

초기:

- 내장 bitmap font
- ASCII 중심 debug UI

중기:

- FreeType
- glyph atlas
- UTF-8 text

후기:

- HarfBuzz
- fallback font
- 한글/다국어 shaping
- IME 대응

---

# Input 정책

## 기본 방향

입력 시스템은 Platform 계층과 Gameplay 계층 사이에 독립 모듈로 둔다.

```text
Platform Native Event
 → Input Backend
 → Input System
 → Action / Axis Mapping
 → Gameplay / UI
```

목표는 다음이다.

- 키보드 입력
- 마우스 입력
- 게임패드 입력
- 터치패드 입력
- 제스처 입력
- UI 입력 라우팅
- 나중에 rebinding 가능한 action mapping

## Platform별 입력 백엔드

1차 플랫폼은 Windows다.

```text
Windows:
  Keyboard: Win32 raw message + optional Raw Input
  Mouse: Win32 message + Raw Input
  Gamepad: XInput first, GameInput later 검토
  Touchpad/Gesture: Windows Pointer / Precision Touchpad 관련 메시지 검토

macOS, later:
  Keyboard / Mouse: Cocoa
  Gamepad: GameController framework
  Touchpad/Gesture: NSEvent gesture

WebGPU, later:
  KeyboardEvent
  PointerEvent
  Gamepad API
  TouchEvent / PointerEvent
```

초기에는 Win32 message 기반으로 시작하고, 마우스 raw delta가 필요해지면 Raw Input을 추가한다.

## 입력 계층

입력은 세 계층으로 나눈다.

```text
Physical Input:
  KeyCode
  MouseButton
  GamepadButton
  TouchId

Logical Input:
  Action
  Axis
  Vector2Axis

Consumer:
  Gameplay
  Editor
  UI
```

예시:

```text
Physical:
  W key

Logical:
  MoveForward

Consumer:
  CameraController
```

## 입력 API 초안

```cpp
enum class KeyCode
{
    A, B, C,
    W, AKey, S, D,
    Escape,
    Space,
    LeftShift,
};

enum class MouseButton
{
    Left,
    Right,
    Middle,
};

struct GamepadState
{
    bool isConnected;
    float leftStickX;
    float leftStickY;
    float rightStickX;
    float rightStickY;
    float leftTrigger;
    float rightTrigger;
};

class InputSystem
{
public:
    void BeginFrame();
    void ProcessPlatformEvent(const PlatformEvent& event);
    void EndFrame();

    bool IsKeyDown(KeyCode key) const;
    bool WasKeyPressed(KeyCode key) const;
    bool WasKeyReleased(KeyCode key) const;

    bool IsMouseButtonDown(MouseButton button) const;
    Vec2 GetMousePosition() const;
    Vec2 GetMouseDelta() const;
    float GetMouseWheelDelta() const;

    const GamepadState& GetGamepad(uint32_t index) const;
};
```

주의: 위 예시의 `AKey`처럼 이름 충돌 가능성이 있는 키 이름은 실제 구현 시 정리한다.

## Action Mapping

초기에는 물리 입력을 직접 조회해도 된다.
하지만 에디터와 게임 입력을 분리하려면 action mapping이 필요하다.

```cpp
struct InputAction
{
    String name;
};

struct InputBinding
{
    InputAction action;
    InputDeviceType deviceType;
    uint32_t code;
};
```

예시:

```text
MoveForward:
  Keyboard W
  Gamepad LeftStickY+

Jump:
  Keyboard Space
  Gamepad A

Look:
  Mouse Delta
  Gamepad RightStick
```

## UI 입력 라우팅

UI 시스템은 raw input을 직접 읽지 않는다.
InputSystem이 정규화한 이벤트를 UI에 전달한다.

```text
InputSystem
 → UiInputEvent
 → UiSystem
 → Focus / Capture / Bubble
```

UI에서 필요한 이벤트:

- PointerMove
- PointerDown
- PointerUp
- PointerWheel
- KeyDown
- KeyUp
- TextInput
- CompositionStart, later
- CompositionUpdate, later
- CompositionEnd, later

한글 IME 대응은 초기 범위에서 제외하되, TextInput과 Composition 이벤트를 나중에 추가할 수 있게 구조를 열어둔다.

## 터치패드 / 제스처

터치패드와 제스처는 1차 구현 대상이 아니라 2차 구현 대상으로 둔다.

우선순위:

```text
1차:
  Keyboard
  Mouse
  Mouse wheel
  XInput gamepad

2차:
  Raw mouse input
  Text input
  Gamepad vibration

3차:
  Touchpad scroll
  Pinch zoom
  Two-finger pan
  Gesture event abstraction
  IME composition
```

제스처는 OS별 차이가 크므로, 엔진 상위에서는 다음처럼 일반화한다.

```cpp
enum class GestureType
{
    Pan,
    Pinch,
    Rotate,
    Tap,
    DoubleTap,
};

struct GestureEvent
{
    GestureType type;
    Vec2 position;
    Vec2 delta;
    float scale;
    float rotation;
};
```

## 입력과 에디터

에디터는 입력 focus를 가진다.

```text
Viewport focused:
  WASD camera movement
  Mouse look
  Gizmo interaction

UI focused:
  Text input
  Button/slider interaction
  Shortcut
```

InputSystem은 입력을 다음 consumer 중 하나로 전달한다.

```text
- UI
- Editor viewport
- Gameplay
```

초기에는 단순 priority 방식으로 처리한다.

```text
UI capture가 있으면 UI 우선
아니면 Editor viewport
아니면 Gameplay
```

## 구현 로드맵

### Phase 1: Basic Input

- Win32 keyboard
- Win32 mouse
- mouse position
- mouse delta
- mouse wheel
- per-frame pressed/released/down state

완료 조건:

- ESC 종료
- WASD 카메라 이동
- 마우스 드래그로 카메라 회전

### Phase 2: Gamepad

- XInput backend
- connection state
- button
- trigger
- stick
- deadzone
- vibration, optional

완료 조건:

- 게임패드로 카메라 이동
- 버튼 입력 확인

### Phase 3: Input Mapping

- Action
- Axis
- binding table
- editor/gameplay profile

완료 조건:

- `MoveForward`, `Look`, `Jump` 같은 logical input 사용

### Phase 4: UI Input

- pointer event
- keyboard focus
- text input 기초
- capture/focus
- shortcut

완료 조건:

- 자체 UI button/slider/text field 입력 가능

### Phase 5: Touchpad / Gesture

- touchpad scroll
- pinch zoom
- pan gesture
- gesture abstraction

완료 조건:

- 에디터 viewport에서 터치패드 pinch zoom / pan 가능

---
