# Yoolmu Engine 개발 계획서

## 문서 목적

이 문서는 **율무엔진(Yoolmu Engine)** 개발을 위한 장기 계획서이다.

개발 조건은 다음을 기준으로 한다.

- 개발자: 1명
- 개발 시간: 하루 1시간 이내
- 목적: 취미용 소형 3D 게임 엔진 + 하드웨어 가속 UI 프레임워크 구현
- 방향: 외부 엔진/프레임워크 의존을 최소화하고, 핵심 구조를 직접 통제한다.
- 1차 목표: Windows 11 + Direct3D 12 기반의 자체 3D 엔진
- 장기 목표: 자체 Asset Pipeline, 자체 UI, 자체 ECS, WASAPI 오디오, Jolt Physics, DirectStorage-ready streaming 구조

---

# 1. 프로젝트 기본 정책

## 1.1 엔진 이름

```text
한국어명: 율무엔진
영문명: Yoolmu Engine
Namespace: Yoolmu
```

`Yoolmu Engine`을 기본 영문명으로 사용한다.

이유:

- 한국어 “율무” 발음에 비교적 가깝다.
- `Yulmu`보다 고유명사화하기 쉽다.
- 네임스페이스 `Yoolmu`로 사용하기 좋다.

---

## 1.2 개발 언어

```text
C++23
```

사용 권장:

- `std::span`
- `std::string_view`
- `std::filesystem`
- `std::source_location`
- `std::expected`, 컴파일러 지원 상태에 따라
- `constexpr` 개선 기능
- `concepts`, 제한적으로

초기 비권장:

- C++20 modules
- coroutine 기반 엔진 코어
- 최신 STL 기능에 과도하게 종속된 runtime hot path

---

## 1.3 1차 플랫폼

```text
Primary Target:
  Windows 11 only

Best-effort:
  Windows 10
  Wine / Proton
```

Windows 11을 정식 타깃으로 한다. Windows 10은 가능하면 동작하게 하되, 정식 지원 대상으로 보장하지 않는다.

Wine/Proton 호환성을 해치지 않도록 다음을 주의한다.

- 불필요한 OS 특수 기능 사용을 피한다.
- Win32/D3D12/WASAPI 사용 경로를 명확히 분리한다.
- fallback 경로를 둔다.
- DirectStorage는 optional capability로 둔다.

---

## 1.4 장기 플랫폼 우선순위

```text
1. Windows + Direct3D 12 + WASAPI
2. macOS + Metal + CoreAudio
3. WebGPU
4. Linux + Vulkan + PipeWire / ALSA
```

Linux는 WebGPU와 마찬가지로 장기 후순위 플랫폼이다. 초기에는 Windows 전용으로 구현한다.

---

# 2. 저장소 정책

## 2.1 Monorepo

Yoolmu Engine은 초기에는 **monorepo-style 단일 저장소**로 구성한다.

```text
YoolmuEngine/
  CMakeLists.txt
  README.md
  LICENSE
  .gitignore

  Docs/
    Architecture/
    Roadmap/
    CodingConvention.md
    AssetPipeline.md
    Rendering.md
    Audio.md
    UI.md
    Input.md
    Memory.md

  Engine/
    CMakeLists.txt
    Source/
      Core/
      Platform/
      Input/
      RHI/
      Render/
      Shader/
      Asset/
      World/
      UI/
      Audio/
      Physics/
      Editor/
    Include/

  Tools/
    CMakeLists.txt
    AssetCooker/
    ShaderCompiler/
    PackageBuilder/
    SceneCooker/

  Samples/
    CMakeLists.txt
    00_Window/
    01_Input/
    02_D3D12Clear/
    03_SlangTriangle/
    04_Cube/
    05_MeshAsset/
    06_TextureAsset/
    07_ECS/
    08_DebugUI/
    09_RenderGraph/
    10_Audio/
    11_Physics/

  Assets/
    Source/
      Models/
      Textures/
      Materials/
      Shaders/
      Scenes/
      Audio/
      Fonts/
    Cooked/
      WindowsD3D12/

  External/
    README.md
    Slang/
    Assimp/
    libpng/
    zlib/
    DirectX-Headers/
    DirectStorage/
    FreeType/
    HarfBuzz/
    SteamAudio/
    JoltPhysics/
    Opus/
    yaml-cpp/

  Scripts/
    build_windows.bat
    cook_assets.bat
    run_sample.bat
    clean.bat

  Build/
    # ignored
```

## 2.2 Monorepo 원칙

- Engine, Tools, Samples, Assets, External을 하나의 저장소에서 관리한다.
- 초기 개발 속도와 빌드 재현성을 우선한다.
- Samples는 항상 Engine 최신 코드와 함께 빌드된다.
- Tools는 Engine의 asset/runtime format 변경과 같은 커밋에서 같이 갱신된다.
- Assets/Source에는 테스트용 소규모 애셋만 포함한다.
- 대용량 애셋은 나중에 Git LFS 또는 별도 저장소로 분리한다.

## 2.3 장기 분리 기준

아래 조건이 생기기 전까지는 monorepo를 유지한다.

- Assets 디렉터리가 과도하게 커짐
- 게임 프로젝트가 엔진과 독립 배포되어야 함
- 외부 사용자를 위한 SDK 형태가 필요함
- Tools와 Engine의 릴리스 주기가 달라짐

---

# 3. 코딩 컨벤션

## 3.1 스타일 방향

C# 스타일을 기본으로 하되, Elixir처럼 의미가 명확한 함수명을 선호한다.

```cpp
namespace Yoolmu::Render
{
    class RenderGraph
    {
    public:
        void AddPass(const RenderPassDesc& desc);
        void Compile();
        void Execute(RenderContext& context);

    private:
        Array<RenderPass> passes_;
    };
}
```

## 3.2 네이밍 규칙

| 대상 | 규칙 | 예시 |
|---|---|---|
| Namespace | PascalCase | `Yoolmu::Render` |
| Class / Struct / Enum | PascalCase | `RenderGraph`, `TextureDesc` |
| Function / Method | PascalCase | `CreateBuffer`, `LoadTexture` |
| Local Variable | camelCase | `frameIndex`, `textureDesc` |
| Member Variable | camelCase_ | `device_`, `frameIndex_` |
| Constant | PascalCase 또는 kPascalCase | `MaxFramesInFlight`, `kMaxFramesInFlight` |
| Enum Value | PascalCase | `ResourceState::ShaderRead` |

## 3.3 예시

```cpp
namespace Yoolmu::Asset
{
    class AssetManager
    {
    public:
        MeshHandle LoadMesh(AssetId assetId);
        TextureHandle LoadTexture(AssetId assetId);

    private:
        HashMap<AssetId, MeshHandle> loadedMeshes_;
        HashMap<AssetId, TextureHandle> loadedTextures_;
    };
}
```

---

# 4. Memory / Allocation 정책

## 4.1 기본 방향

메모리 할당자는 별도 모듈로 구현한다.

단, 초기부터 모든 STL과 모든 new/delete를 제거하지 않는다.
1년차 초반에는 표준 할당을 허용하고, 성능 문제가 발생하는 hot path부터 점진적으로 전용 allocator를 적용한다.

```text
Initial:
  std::vector / std::string / new / delete 허용

Middle:
  FrameAllocator
  LinearAllocator
  StackAllocator
  PoolAllocator
  FreeListAllocator

Later:
  TrackingAllocator
  Tag-based allocation statistics
  Per-system memory budget
  Custom containers
```

## 4.2 구현 목적

메모리 시스템의 목적은 다음이다.

- 프레임 단위 임시 할당 비용 감소
- 렌더링/애셋/월드 시스템의 메모리 사용량 추적
- cache locality 개선
- fragmentation 감소
- allocation hot spot 탐지
- 장기적으로 자체 container와 연결

## 4.3 초기 Memory 모듈 구성

```text
Core/Memory/
  Memory.h
  Memory.cpp
  Allocator.h
  LinearAllocator.h
  StackAllocator.h
  PoolAllocator.h
  FrameAllocator.h
  MemoryTracker.h
```

## 4.4 Allocator 종류

### FrameAllocator

매 프레임 초기화되는 임시 메모리에 사용한다.

사용처:

- RenderExtract 임시 배열
- UI paint command buffer
- frame graph build data
- temporary string formatting
- transient CPU-side upload preparation

```cpp
class FrameAllocator
{
public:
    void* Allocate(size_t size, size_t alignment);
    void Reset();
};
```

### LinearAllocator

한 방향으로만 할당하고 한 번에 해제하는 allocator다.

사용처:

- Asset import intermediate data
- Shader reflection temporary data
- Scene loading temporary data

### PoolAllocator

고정 크기 객체를 빠르게 할당/해제하는 allocator다.

사용처:

- Entity metadata
- UI node
- Audio voice
- Render pass object
- Job object

### FreeListAllocator

크기가 다양한 장수명 객체에 사용한다.

사용처:

- Runtime asset object
- Material instance
- Persistent editor object

### TrackingAllocator

디버깅과 통계를 위한 wrapper allocator다.

기록 항목:

- allocation count
- total allocated bytes
- peak allocated bytes
- allocation tag
- file / line, 가능하면
- leak report

## 4.5 적용 순서

초기부터 모든 것을 allocator 기반으로 만들지 않는다.

```text
1단계:
  MemoryTracker만 구현
  new/delete 또는 malloc/free wrapper로 통계 수집

2단계:
  FrameAllocator 구현
  UI / RenderGraph / RenderExtract 임시 메모리에 적용

3단계:
  PoolAllocator 구현
  AudioVoice / UI node / RenderPass에 적용

4단계:
  AssetCooker와 runtime asset loading에 LinearAllocator 적용

5단계:
  자체 Array / HashMap / String이 필요해질 때 allocator-aware container로 확장
```

## 4.6 하지 않을 것

초기에는 다음을 하지 않는다.

- 전역 new/delete 완전 교체
- STL 완전 제거
- 복잡한 TLSF allocator
- lock-free general-purpose allocator
- OS virtual memory 기반 대형 allocator
- NUMA-aware allocator

이들은 성능 측정 후 필요할 때 추가한다.

## 4.7 성능 원칙

```text
먼저 측정한다.
그 다음 hot path에 allocator를 넣는다.
마지막으로 container를 바꾼다.
```

메모리 allocator는 성능을 낼 수 있지만, 너무 빨리 전면 적용하면 디버깅 비용이 커진다.
따라서 **FrameAllocator + MemoryTracker를 1차 목표**로 삼는다.

---

# 5. 좌표계 / 행렬 / 단위

## 4.1 좌표계

```text
Coordinate System:
  Left-Handed

Axis:
  +X: right
  +Y: up
  +Z: forward
```

Direct3D 스타일의 Left-Handed world를 사용한다.

## 4.2 행렬 규약

```text
Matrix Convention:
  Row-major
  Direct3D convention
  Row-vector style
```

CPU와 shader 모두 row-major matrix를 기본으로 사용한다.

```cpp
Vec4 clip = position * world * view * projection;
```

Slang/HLSL 셰이더에서도 동일한 규약을 유지한다.

```hlsl
float4 clipPosition = mul(float4(position, 1.0), WorldViewProjection);
```

## 4.3 단위계

```text
1 engine unit = 1 meter
```

예시:

| 대상 | 값 |
|---|---:|
| Character height | 1.7 units |
| Door height | 2.0 units |
| Gravity | -9.81 m/s² |
| Walk speed | 3.0 m/s |
| Audio distance unit | meter |
| Physics unit | meter |

---

# 6. 외부 라이브러리 정책

## 5.1 허용 라이브러리

### Core Allowed

- Slang
- Assimp, Tools only
- libpng
- zlib / miniz
- DirectX-Headers
- DirectStorage SDK
- FreeType
- HarfBuzz
- Steam Audio
- Jolt Physics
- Opus
- yaml-cpp, Tools/Editor only

### Conditional

- DDS/KTX helper, 필요 시 Tools only
- Dotori, 2년차 이후 검토

### Not Allowed

- Dear ImGui
- SDL / GLFW
- EnTT / Flecs
- bgfx / The Forge / Diligent Engine
- glm
- spdlog / fmt
- cereal / rapidjson
- Runtime YAML parser

## 5.2 원칙

외부 라이브러리는 다음 조건을 만족할 때만 허용한다.

- 직접 구현하면 개발 기간이 과도하게 길어진다.
- 엔진 구조를 대신 결정하지 않는다.
- Runtime hot path를 불필요하게 오염시키지 않는다.
- Tools-only로 제한 가능한 경우 Runtime에 링크하지 않는다.

---

# 7. 그래픽 정책

## 6.1 Graphics API

```text
Windows:
  Direct3D 12 only

Not supported:
  Direct3D 11
```

Direct3D 11은 지원하지 않는다.

이유:

- RHI가 두꺼워짐
- DirectStorage와 궁합이 약함
- D3D12/Vulkan/Metal/WebGPU 계열 explicit API 구조와 맞지 않음
- 하루 1시간 개발 기준에서 유지보수 비용이 큼

## 6.2 Vulkan 정책

Vulkan은 D3D12 백엔드가 안정된 뒤 Windows에서 RHI 검증용으로 추가한다.

```text
D3D12 먼저 완성
→ RHI 정리
→ Vulkan으로 RHI 검증
→ 그 뒤 macOS/Metal 또는 Linux 검토
```

---

# 8. RHI 정책

## 7.1 RHI 목표

RHI는 D3D12/Vulkan/Metal/WebGPU 계열의 explicit API에 맞춘다.

상위 API에서는 D3D12 descriptor heap을 직접 노출하지 않는다.

좋은 상위 모델:

```cpp
cmd.SetPipeline(pipeline);
cmd.SetBindGroup(0, frameResources);
cmd.SetBindGroup(1, materialResources);
cmd.DrawIndexed(indexCount);
```

피해야 할 모델:

```cpp
cmd.SetDescriptorHeap(heap);
cmd.SetGpuDescriptorHandle(slot, handle);
```

## 7.2 RHI 주요 객체

```text
RHI
 ├─ Device
 ├─ Queue
 ├─ CommandList
 ├─ Buffer
 ├─ Texture
 ├─ Sampler
 ├─ Shader
 ├─ Pipeline
 ├─ BindGroup
 ├─ Swapchain
 ├─ Fence
 └─ DescriptorAllocator, D3D12 backend internal
```

## 7.3 구현 순서

```text
Raw D3D12 sample
→ 반복 코드 wrapper화
→ Buffer / Texture / Pipeline / CommandList wrapper
→ Upload ring buffer
→ Descriptor allocator
→ BindGroup-style abstraction
→ Render Graph
```

---

# 9. 렌더러 정책

## 8.1 초기 렌더러

```text
Initial:
  Forward Renderer

Later:
  Forward+

Deferred:
  후순위
```

초기에는 단순 Forward Renderer를 사용한다. 이후 light 수가 늘어나면 Forward+로 확장한다.

## 8.2 머티리얼 모델

초기 머티리얼은 간단한 metallic/roughness PBR이다.

초기 범위:

- baseColor
- normal
- metallic
- roughness
- directional light 1개

후속 범위:

- shadow map
- point light
- spot light
- IBL
- Forward+ light culling
- postprocess

## 8.3 GPU-driven 장기 목표

장기적으로 다음을 고려한다.

- GPU scene
- indirect draw
- compute culling
- meshlet / cluster renderer
- bindless-style resource indexing
- async compute, 후순위

---

# 10. Slang / Shader Pipeline

## 9.1 기본 정책

```text
Shader Language:
  Slang
```

Asset Pipeline을 초기에 도입하므로 ShaderCompiler 툴을 처음부터 만든다.

다만 개발 편의를 위해 Debug/Development 빌드에서는 runtime compile 경로를 임시 허용할 수 있다.

```text
Development:
  .slang → runtime compile 허용

Runtime/Shipping:
  .shaderbin만 로드
```

## 9.2 ShaderCompiler

```text
ShaderCompiler
 ├─ Slang source input
 ├─ Entry point
 ├─ Target: DXIL initially
 ├─ Reflection extraction
 ├─ Pipeline layout metadata
 └─ .shaderbin output
```

## 9.3 WebGPU portable subset

초기에는 D3D12 우선으로 작성한다.

WebGPU/WGSL portable subset은 초기에 강제하지 않는다.
단, 기본 렌더러에서는 다음을 피한다.

- geometry shader
- tessellation shader
- mesh shader 기본 경로 의존
- wave intrinsic 기본 경로 의존
- ray tracing 기본 경로 의존

나중에 WebGPU 대응 시 다음처럼 분리한다.

```text
Shaders/
  Portable/
    MeshForward.slang
    Ui.slang
    ShadowMap.slang

  Native/
    Meshlet.slang
    GpuCulling.slang
    RayTracing.slang
```

---

# 11. Asset Pipeline

## 10.1 기본 정책

Asset Pipeline은 후순위가 아니다.
1년차 중반부터 도입한다.

```text
Source Assets
 → Tools/AssetCooker
 → Cooked Assets
 → Runtime
```

Runtime은 source asset을 직접 읽지 않는다.

## 10.2 Assimp 정책

Assimp는 Tools/AssetCooker 전용으로 사용한다.

```text
FBX / OBJ / glTF / 기타
 → Assimp
 → RawModel
 → .meshbin / .matbin / .skelbin / .animbin
```

Runtime은 Assimp에 의존하지 않는다.

## 10.3 Model Import

Assimp가 읽을 수 있는 여러 포맷을 best-effort로 수용한다.

공식 검증 우선순위:

```text
1. glTF / GLB
2. OBJ
3. FBX best-effort
```

FBX는 공식 1차 포맷으로 보장하지 않는다. 변환 문제를 허용한다.

## 10.4 Runtime asset formats

```text
.meshbin
.matbin
.texbin
.shaderbin
.scenebin
.audiobin
.pak, later
.skelbin, later
.animbin, later
```

---

# 12. Texture Pipeline

## 11.1 Source Texture Format

초기 AssetCooker는 다음 source texture format을 처리한다.

```text
PNG
TGA
DDS
KTX
```

## 11.2 Runtime Texture Format

Runtime은 source image를 직접 읽지 않는다.

```text
Source texture
 → TextureCompiler
 → .texbin
 → Runtime
```

또는 package 도입 이후:

```text
Source texture
 → TextureCompiler
 → package chunk
 → Runtime
```

## 11.3 GPU Compression

BCn 압축은 2년차 이후 도입한다.

초기에는 RGBA8 중심으로 구현한다.

```text
Initial:
  RGBA8

Later:
  BC1
  BC3
  BC5
  BC7
```

DDS/KTX는 처음에는 container import 수준으로 제한한다. 지원하지 않는 format은 명시적으로 실패한다.

---

# 13. ECS / World

## 12.1 기본 정책

초기 ECS는 sparse set component pool 방식으로 구현한다.

```text
Initial:
  Sparse set ECS

Later:
  Hot component SoA storage
  Archetype ECS 검토
```

전체 ECS를 처음부터 archetype으로 구현하지 않는다.

## 12.2 Entity

```cpp
struct Entity
{
    uint32_t index;
    uint32_t generation;
};
```

## 12.3 Component

초기 컴포넌트:

- TransformComponent
- MeshRendererComponent
- CameraComponent
- LightComponent
- AudioSourceComponent
- PhysicsBodyComponent, later
- UiComponent, later

## 12.4 GameObject Facade

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

## 12.5 RenderExtract

Renderer는 GameObject를 직접 참조하지 않는다.

```text
ECS World
 → RenderExtract
 → RenderWorld / GPU Scene
 → RenderGraph
```

---

# 14. Scene Format

## 13.1 초기

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

## 13.2 Editor / Tools

에디터와 애셋 파이프라인이 안정된 뒤 YAML로 전환한다.

```text
YAML:
  Tools/Editor only

Runtime:
  YAML 직접 로딩 금지
```

`yaml-cpp`는 Tools/Editor only로 허용한다.

## 13.3 Runtime

Runtime은 최종적으로 cooked scene binary만 읽는다.

```text
scene.yaml
 → SceneCooker
 → .scenebin
 → Runtime
```

---

# 15. UI 정책

## 14.1 기본 정책

Dear ImGui는 사용하지 않는다.

자체 UI 시스템을 구현한다.

최종 목표는 Editor UI와 Runtime UI를 하나의 UI 시스템으로 처리하는 것이다.

## 14.2 구현 전략

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

## 14.3 Text

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

# 16. Input 정책

## 16.1 기본 방향

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

## 16.2 Platform별 입력 백엔드

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

## 16.3 입력 계층

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

## 16.4 입력 API 초안

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

## 16.5 Action Mapping

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

## 16.6 UI 입력 라우팅

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

## 16.7 터치패드 / 제스처

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

## 16.8 입력과 에디터

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

## 16.9 구현 로드맵

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

# 17. Audio 정책

## 15.1 기본 정책

Windows 1차 오디오 백엔드는 WASAPI shared mode다.

```text
Windows:
  WASAPI shared mode

macOS:
  CoreAudio, later

Linux:
  PipeWire / ALSA, later
```

Linux 오디오는 WebGPU처럼 후순위다.

## 15.2 엔진 오디오 구조

OS 백엔드는 최종 출력 장치일 뿐이다.

```text
AudioSource
 → Decoder
 → Mixer
 → Spatial Processor
 → Master Bus
 → WASAPI
```

엔진은 자체 AudioMixer를 가진다.

## 15.3 내부 포맷

```text
Sample format:
  float32

Recommended default:
  48 kHz
  stereo
  interleaved float32
```

초기에는 interleaved float32를 사용한다.

## 15.4 초기 범위

- WASAPI shared mode
- WAV PCM loader
- Opus decoder
- AudioSource
- AudioVoice
- AudioBus
- Master/SFX/BGM volume
- looping
- play/stop

## 15.5 Steam Audio

Steam Audio는 spatial audio backend로 사용한다.

```text
Steam Audio:
  HRTF
  occlusion
  reflection
  reverb
```

Steam Audio는 출력 백엔드가 아니다.

```text
Playback backend:
  WASAPI / CoreAudio / PipeWire / ALSA

Spatial backend:
  Steam Audio
```

---

# 18. Physics 정책

## 16.1 기본 정책

물리 엔진은 직접 구현하지 않는다.

1차 후보는 Jolt Physics다.

## 16.2 ECS 연동

ECS와 물리 엔진 내부 body storage는 분리한다.

```text
ECS:
  PhysicsBodyComponent
  PhysicsShapeComponent
  TransformComponent

Jolt:
  BodyID
  Shape
  MotionType
  PhysicsWorld
```

동기화:

```text
Before simulation:
  ECS → Physics

After simulation:
  Physics → ECS
```

## 16.3 장기 검토

Fully-parallel physics engine 후보는 장기적으로 비교한다.

단, 초기에는 Jolt Physics 통합을 우선한다.

---

# 19. Scripting 정책

초기에는 스크립팅을 넣지 않는다.

```text
Initial:
  C++ only

Later:
  Lua or C# 검토 가능
```

스크립팅은 범위 폭발 위험이 있으므로 후순위로 둔다.

---

# 20. Build System

## 18.1 초기

1년차는 CMake로 고정한다.

```text
Build System:
  CMake
```

## 18.2 Dotori

Dotori는 2년차 이후 검토한다.

```text
1년차:
  CMake

2년차 이후:
  Dotori 도입 검토
```

Dotori를 도입하더라도 CMake fallback 또는 export 경로를 유지한다.

---

# 21. DirectStorage 정책

## 19.1 기본 정책

DirectStorage는 package/streaming 구조가 안정된 뒤 도입한다.

1차 목표:

```text
uncompressed package chunk → GPU buffer
```

## 19.2 단계

```text
1. Package format
2. CPU async streaming
3. Storage abstraction
4. DirectStorage uncompressed buffer path
5. GDeflate
6. Texture mip streaming
```

## 19.3 Fallback

CPU fallback은 반드시 유지한다.

```text
DirectStorage available:
  package chunk → DirectStorage → GPU buffer

Fallback:
  package chunk → CPU read → upload buffer → GPU buffer
```

---

# 22. Web / Linux 정책

## 20.1 Web

Web은 WebGPU만 장기 고려한다.

```text
Supported later:
  WebGPU

Not supported:
  WebGL 2
```

초기에는 WebGPU를 구현하지 않는다.

구조적으로 다음은 막지 않는다.

- Platform abstraction
- RHI abstraction
- Storage abstraction
- Shader target abstraction
- Application::Tick() 구조

## 20.2 Linux

Linux는 후순위 플랫폼이다.

장기 후보:

```text
Graphics:
  Vulkan

Audio:
  PipeWire
  ALSA fallback
```

초기에는 구현하지 않는다.

---

# 23. 1년차 로드맵

## Month 1: Project Foundation

목표:

- CMake 기반 프로젝트 생성
- Win32 window
- Logger
- Assert
- Timer
- Basic file utility
- Win32 keyboard/mouse input 기초

완료 조건:

- Windows 11에서 창 생성
- ESC 종료
- 로그 출력
- 키보드/마우스 상태 확인

---

## Month 2: Math + Core Types

목표:

- Vec2 / Vec3 / Vec4
- Mat4
- Quaternion 기본
- AABB
- Color
- StringView / Array는 STL 기반으로 시작 가능
- MemoryTracker 초안
- FrameAllocator 초안

완료 조건:

- 큐브 렌더링에 필요한 math 준비
- 프레임 임시 메모리 할당 실험 가능

---

## Month 3: D3D12 Clear

목표:

- DXGI factory
- Adapter selection
- D3D12 device
- Command queue/list/allocator
- Swapchain
- RTV heap
- Fence
- Backbuffer clear

완료 조건:

- D3D12로 화면 clear

---

## Month 4: Slang + Triangle

목표:

- Slang 연동
- ShaderCompiler 초기 툴
- 개발용 runtime compile path
- Root signature
- Pipeline state
- Vertex buffer
- Triangle draw

완료 조건:

- Slang 셰이더로 삼각형 출력

---

## Month 5: Cube + Depth + Constant Buffer

목표:

- Index buffer
- Constant buffer
- Depth buffer
- MVP matrix
- Camera
- Rotating cube

완료 조건:

- 회전하는 3D 큐브

---

## Month 6: D3D12 Foundation Wrappers

목표:

- Buffer wrapper
- Texture wrapper
- Pipeline wrapper
- CommandList wrapper
- Descriptor allocator
- Upload ring buffer
- Frame resource

완료 조건:

- Raw D3D12 반복 코드 감소
- Cube sample이 wrapper 기반으로 동작

---

## Month 7: Asset Cooker 1차

목표:

- Tools/AssetCooker 생성
- Assimp import 연결
- RawModel 정규화
- .meshbin writer
- .shaderbin writer 기초

완료 조건:

- Assimp로 읽은 모델을 .meshbin으로 변환

---

## Month 8: Mesh/Material Runtime

목표:

- .meshbin runtime loader
- .matbin format
- Material parameter
- Basic PBR shader
- Assimp material slot 변환

완료 조건:

- cooked mesh 렌더링

---

## Month 9: Texture Pipeline

목표:

- PNG/TGA/DDS/KTX source import
- .texbin format
- D3D12 texture upload
- BaseColor texture binding
- Sampler

완료 조건:

- 텍스처가 입혀진 cooked model 출력

---

## Month 10: ECS 1차

목표:

- Entity generation
- Sparse set component pool
- TransformComponent
- MeshRendererComponent
- CameraComponent
- RenderExtract

완료 조건:

- ECS entity 여러 개 렌더링

---

## Month 11: 자체 UI 1차

목표:

- UI command
- Rect rendering
- Text rendering, 초기 bitmap font
- Button
- Slider
- Checkbox
- Debug panel

완료 조건:

- Dear ImGui 없이 디버그 UI 표시

---

## Month 12: Integration Demo

목표:

- Basic scene
- Entity selection 준비
- Camera control
- Material parameter 조정
- 문서화
- v0.1 demo

완료 조건:

- Windows 11 + D3D12 + Slang + cooked asset + ECS + 자체 UI 데모

---

# 24. 2년차 로드맵

## Quarter 1: Render Graph

목표:

- Pass registration
- Resource read/write declaration
- Backbuffer import
- Depth/resource barrier helper
- UI pass integration

완료 조건:

- Clear Pass
- Mesh Pass
- UI Pass
- Present Pass

---

## Quarter 2: Editor Foundation

목표:

- 자체 UI 확장
- Hierarchy panel
- Inspector panel
- Transform editing
- Material editing
- 자체 text scene format

완료 조건:

- Entity 선택
- Transform 수정
- Material parameter 수정
- Scene 저장/로드 초기 버전

---

## Quarter 3: Lighting / Shadow / PBR 개선

목표:

- Directional light
- Point light 기초
- Shadow map
- Normal map
- Metallic/roughness 개선
- IBL 준비

완료 조건:

- textured mesh에 조명과 그림자 적용

---

## Quarter 4: Audio + Physics 1차

목표:

- WASAPI shared backend
- WAV PCM loader
- Opus decoder
- 자체 mixer
- Jolt Physics 통합
- ECS PhysicsBodyComponent

완료 조건:

- 효과음/BGM 재생
- 기본 rigid body 시뮬레이션
- ECS와 physics sync

---

# 25. 3년차 로드맵

## Quarter 1: Package Format

목표:

- .pak format
- AssetTable
- ChunkTable
- DependencyTable
- BulkData
- cooked scene binary

완료 조건:

- 여러 cooked asset을 하나의 package에서 로드

---

## Quarter 2: Async Streaming

목표:

- Storage abstraction
- Background loading
- Request queue
- Completion queue
- CPU fallback upload path

완료 조건:

- 로딩 중 프레임 유지
- placeholder asset 사용 가능

---

## Quarter 3: DirectStorage 1차

목표:

- DirectStorage initialization
- Package file open
- Uncompressed chunk → GPU buffer
- Fence/completion integration
- CPU fallback

완료 조건:

- DirectStorage 경로와 CPU fallback 경로 결과 동일

---

## Quarter 4: Platform Expansion / Vulkan Preparation

목표:

- Vulkan backend prototype on Windows
- RHI 검증
- Shader target abstraction 개선
- WebGPU/macOS/Linux 장기 준비 문서화

완료 조건:

- D3D12 중심 RHI가 다른 explicit backend에도 대응 가능한지 검증

---

# 26. 샘플 정책

모든 기능은 샘플로 검증한다.

```text
Samples/
  00_Window
  01_Input
  02_D3D12Clear
  03_SlangTriangle
  03_Cube
  04_MeshAsset
  05_TextureAsset
  06_ECS
  07_DebugUI
  08_RenderGraph
  09_Audio
  10_Physics
  11_Package
  12_DirectStorage
```

샘플이 없으면 기능이 완성된 것이 아니다.

---

# 27. 하루 1시간 작업 원칙

## 좋은 작업 단위

- Vec3::Cross 구현
- Win32 key state 저장
- mouse delta 계산
- FrameAllocator::Allocate 구현
- D3D12 RTV heap 생성
- Slang compile error 출력
- MeshFileHeader 저장
- UI rect 하나 그리기
- Entity generation 체크 추가
- WAV header parser 작성

## 나쁜 작업 단위

- 에디터 만들기
- ECS 완성하기
- UI 시스템 구현하기
- DirectStorage 붙이기
- 애셋 파이프라인 만들기
- RHI 전체 리팩터링

작업은 반드시 1시간 안에 완료 가능한 크기로 쪼갠다.

---

# 28. 매주 / 매월 점검

## 매일 체크

- 오늘 작업이 1시간 안에 끝나는가?
- 실패해도 main branch가 깨지지 않는가?
- 샘플로 확인 가능한가?

## 매주 체크

- 실행 가능한 샘플이 있는가?
- 지난주보다 눈에 보이는 변화가 있는가?
- 불필요한 추상화가 늘지 않았는가?
- 문서가 코드와 크게 어긋나지 않는가?

## 매월 체크

- 이번 달 데모가 있는가?
- 다음 달 목표가 너무 크지 않은가?
- 가장 위험한 기술 문제가 무엇인가?
- 계속 만들 동기가 유지되는가?

---

# 29. 최종 목표 문장

Yoolmu Engine의 목표는 다음이다.

```text
Windows 11 중심의 D3D12 전용 소형 3D 엔진.

Slang 기반 셰이더 파이프라인, Assimp 기반 Tools-only 모델 임포터,
자체 cooked asset format, 자체 sparse-set ECS, 자체 UI,
WASAPI 기반 오디오, Jolt Physics, DirectStorage-ready asset streaming을 목표로 한다.

외부 라이브러리는 엔진 구조를 대체하지 않는 기반/툴 라이브러리만 허용한다.
Dear ImGui, SDL/GLFW, EnTT/Flecs, glm, bgfx류는 사용하지 않는다.
```

성공 기준은 기능 수가 아니라 다음이다.

```text
- 내가 만든 포맷으로 모델이 뜬다.
- 내가 만든 ECS로 오브젝트가 움직인다.
- 내가 만든 UI로 디버그 값을 조정한다.
- 내가 만든 Render Graph로 프레임이 구성된다.
- 내가 만든 Storage Layer로 애셋이 GPU까지 간다.
- 하루 1시간으로도 계속 전진할 수 있다.
```
