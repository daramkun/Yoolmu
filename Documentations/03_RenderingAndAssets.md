# 그래픽 정책

## Graphics API

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

## Vulkan 정책

Vulkan은 D3D12 백엔드가 안정된 뒤 Windows에서 RHI 검증용으로 추가한다.

```text
D3D12 먼저 완성
→ RHI 정리
→ Vulkan으로 RHI 검증
→ 그 뒤 macOS/Metal 또는 Linux 검토
```

---

# RHI 정책

## RHI 목표

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

## RHI 주요 객체

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

## 구현 순서

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

# 렌더러 정책

## 초기 렌더러

```text
Initial:
  Forward Renderer

Later:
  Forward+

Deferred:
  후순위
```

초기에는 단순 Forward Renderer를 사용한다. 이후 light 수가 늘어나면 Forward+로 확장한다.

## 머티리얼 모델

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

## GPU-driven 장기 목표

장기적으로 다음을 고려한다.

- GPU scene
- indirect draw
- compute culling
- meshlet / cluster renderer
- bindless-style resource indexing
- async compute, 후순위

---

# Slang / Shader Pipeline

## 기본 정책

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

## ShaderCompiler

```text
ShaderCompiler
 ├─ Slang source input
 ├─ Entry point
 ├─ Target: DXIL initially
 ├─ Reflection extraction
 ├─ Pipeline layout metadata
 └─ .shaderbin output
```

## WebGPU portable subset

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

# Asset Pipeline

## 기본 정책

Asset Pipeline은 후순위가 아니다.
1년차 중반부터 도입한다.

```text
Source Assets
 → Tools/AssetCooker
 → Cooked Assets
 → Runtime
```

Runtime은 source asset을 직접 읽지 않는다.

## Assimp 정책

Assimp는 Tools/AssetCooker 전용으로 사용한다.

```text
FBX / OBJ / glTF / 기타
 → Assimp
 → RawModel
 → .meshbin / .matbin / .skelbin / .animbin
```

Runtime은 Assimp에 의존하지 않는다.

## Model Import

Assimp가 읽을 수 있는 여러 포맷을 best-effort로 수용한다.

공식 검증 우선순위:

```text
1. glTF / GLB
2. OBJ
3. FBX best-effort
```

FBX는 공식 1차 포맷으로 보장하지 않는다. 변환 문제를 허용한다.

## Runtime asset formats

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

# Texture Pipeline

## Source Texture Format

초기 AssetCooker는 다음 source texture format을 처리한다.

```text
PNG
TGA
DDS
KTX
```

## Runtime Texture Format

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

## GPU Compression

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
