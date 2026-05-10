# Memory / Allocation 정책

## 기본 방향

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

## 구현 목적

메모리 시스템의 목적은 다음이다.

- 프레임 단위 임시 할당 비용 감소
- 렌더링/애셋/월드 시스템의 메모리 사용량 추적
- cache locality 개선
- fragmentation 감소
- allocation hot spot 탐지
- 장기적으로 자체 container와 연결

## 초기 Memory 모듈 구성

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

## Allocator 종류

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

## 적용 순서

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

## 하지 않을 것

초기에는 다음을 하지 않는다.

- 전역 new/delete 완전 교체
- STL 완전 제거
- 복잡한 TLSF allocator
- lock-free general-purpose allocator
- OS virtual memory 기반 대형 allocator
- NUMA-aware allocator

이들은 성능 측정 후 필요할 때 추가한다.

## 성능 원칙

```text
먼저 측정한다.
그 다음 hot path에 allocator를 넣는다.
마지막으로 container를 바꾼다.
```

메모리 allocator는 성능을 낼 수 있지만, 너무 빨리 전면 적용하면 디버깅 비용이 커진다.
따라서 **FrameAllocator + MemoryTracker를 1차 목표**로 삼는다.

---

# 좌표계 / 행렬 / 단위

## 좌표계

```text
Coordinate System:
  Left-Handed

Axis:
  +X: right
  +Y: up
  +Z: forward
```

Direct3D 스타일의 Left-Handed world를 사용한다.

## 행렬 규약

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

## 단위계

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

# 외부 라이브러리 정책

## 허용 라이브러리

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

## 원칙

외부 라이브러리는 다음 조건을 만족할 때만 허용한다.

- 직접 구현하면 개발 기간이 과도하게 길어진다.
- 엔진 구조를 대신 결정하지 않는다.
- Runtime hot path를 불필요하게 오염시키지 않는다.
- Tools-only로 제한 가능한 경우 Runtime에 링크하지 않는다.

---
