# 프로젝트 기본 정책

## 엔진 이름

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

## 개발 언어

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

## 1차 플랫폼

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

## 장기 플랫폼 우선순위

```text
1. Windows + Direct3D 12 + WASAPI
2. macOS + Metal + CoreAudio
3. WebGPU
4. Linux + Vulkan + PipeWire / ALSA
```

Linux는 WebGPU와 마찬가지로 장기 후순위 플랫폼이다. 초기에는 Windows 전용으로 구현한다.

---

# 저장소 정책

## Monorepo

Yoolmu Engine은 초기에는 **monorepo-style 단일 저장소**로 구성한다.

```text
YoolmuEngine/
  CMakeLists.txt
  README.md
  LICENSE
  .gitignore

  Documentations/
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
    12_Package/
    13_DirectStorage/

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

## Monorepo 원칙

- Engine, Tools, Samples, Assets, External을 하나의 저장소에서 관리한다.
- 초기 개발 속도와 빌드 재현성을 우선한다.
- Samples는 항상 Engine 최신 코드와 함께 빌드된다.
- Tools는 Engine의 asset/runtime format 변경과 같은 커밋에서 같이 갱신된다.
- Assets/Source에는 테스트용 소규모 애셋만 포함한다.
- 대용량 애셋은 나중에 Git LFS 또는 별도 저장소로 분리한다.

## 장기 분리 기준

아래 조건이 생기기 전까지는 monorepo를 유지한다.

- Assets 디렉터리가 과도하게 커짐
- 게임 프로젝트가 엔진과 독립 배포되어야 함
- 외부 사용자를 위한 SDK 형태가 필요함
- Tools와 Engine의 릴리스 주기가 달라짐

---

# 코딩 컨벤션

## 스타일 방향

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

## 네이밍 규칙

| 대상 | 규칙 | 예시 |
|---|---|---|
| Namespace | PascalCase | `Yoolmu::Render` |
| Class / Struct / Enum | PascalCase | `RenderGraph`, `TextureDesc` |
| Function / Method | PascalCase | `CreateBuffer`, `LoadTexture` |
| Local Variable | camelCase | `frameIndex`, `textureDesc` |
| Member Variable | camelCase_ | `device_`, `frameIndex_` |
| Constant | PascalCase 또는 kPascalCase | `MaxFramesInFlight`, `kMaxFramesInFlight` |
| Enum Value | PascalCase | `ResourceState::ShaderRead` |

## 예시

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
