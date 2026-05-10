# 1년차 로드맵

## 1년차 운영 원칙

개발 조건:

- 개발자 1명
- 하루 1시간 이내
- Windows 11 + C++23 + Direct3D 12 우선
- 기능 완성보다 작은 샘플의 반복 검증을 우선
- Runtime은 source asset을 직접 읽지 않고, Tools가 만든 cooked asset을 읽는 방향으로 조기 수렴

월별 산출물 기준:

- 매월 최소 1개 실행 가능한 Sample을 남긴다.
- 완료 조건은 눈으로 확인 가능한 결과와 로그/테스트로 확인 가능한 결과를 함께 둔다.
- 다음 달 작업이 이전 달 결과 위에서 바로 시작될 수 있도록 API와 파일 배치를 정리한다.
- 큰 리팩터링은 월말에만 한다. 월중에는 샘플을 깨지 않는 범위에서만 수정한다.

권장 저장소 체크포인트:

- `Samples/00_Window`
- `Samples/01_Input`
- `Samples/02_D3D12Clear`
- `Samples/03_SlangTriangle`
- `Samples/04_Cube`
- `Samples/05_MeshAsset`
- `Samples/06_TextureAsset`
- `Samples/07_ECS`
- `Samples/08_DebugUI`

---

## Month 1: Project Foundation

목표:

- CMake 기반 프로젝트 생성
- Win32 window
- Logger
- Assert
- Timer
- Basic file utility
- Win32 keyboard/mouse input 기초

세부 작업:

- 저장소 기본 구조를 만든다.
  - `Engine/Source/Core`
  - `Engine/Source/Platform`
  - `Engine/Source/Input`
  - `Samples/00_Window`
  - `Samples/01_Input`
  - `Tools`
  - `Assets/Source`
  - `Assets/Cooked`
- 루트 `CMakeLists.txt`에서 Engine, Tools, Samples를 함께 빌드할 수 있게 한다.
- Windows 전용 platform layer 초안을 만든다.
  - window class 등록
  - Win32 message loop
  - resize/minimize 처리
  - high DPI 정책은 문서화만 하고 초기 구현은 단순화
- Logger를 만든다.
  - console 출력
  - Visual Studio OutputDebugString 출력
  - log level: Trace, Info, Warning, Error, Fatal
  - source location 포함
- Assert 계층을 만든다.
  - debug break
  - message 포함
  - fatal assert 시 로그 출력
- Timer를 만든다.
  - `QueryPerformanceCounter` 기반
  - frame delta time
  - elapsed time
- File utility를 만든다.
  - binary read/write
  - text read
  - path exists
  - directory create
- Win32 input 초안을 만든다.
  - key down/up
  - mouse button down/up
  - mouse position
  - mouse wheel
  - per-frame pressed/released 상태

산출물:

- `Samples/00_Window`: 창 생성, ESC 종료, 로그 출력
- `Samples/01_Input`: 키보드/마우스 상태를 로그 또는 창 제목으로 확인
- `Scripts/build_windows.bat` 또는 동등한 빌드 스크립트 초안

완료 조건:

- Windows 11에서 창 생성
- ESC 종료
- 로그 출력
- 키보드/마우스 상태 확인
- Debug/Release 빌드가 모두 통과

주의할 점:

- 이 단계에서 cross-platform abstraction을 과하게 만들지 않는다.
- Win32와 Engine public API의 경계만 분리한다.
- 입력은 action mapping 없이 physical state 조회로 시작한다.

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

세부 작업:

- Math 타입을 만든다.
  - `Vec2`, `Vec3`, `Vec4`
  - `Mat4`
  - `Quat`
  - `AABB`
  - `Color`
- 렌더링에 필요한 최소 연산을 우선 구현한다.
  - dot/cross
  - normalize
  - length
  - matrix multiply
  - translation/rotation/scale matrix
  - perspective projection
  - look-at view matrix
  - quaternion from axis-angle
- 좌표계 정책을 문서화한다.
  - handedness
  - clip space
  - matrix memory layout
  - row-major/column-major 사용 규칙
- Core container 정책을 정한다.
  - 초기에는 STL 사용
  - hot path 전용 container는 나중에 도입
  - `std::span`, `std::string_view`, `std::filesystem` 사용 허용
- MemoryTracker 초안을 만든다.
  - allocation count
  - total allocated bytes
  - debug label
  - leak summary 출력
- FrameAllocator 초안을 만든다.
  - linear allocation
  - alignment 처리
  - frame end reset
  - overflow assert

산출물:

- `Engine/Source/Core/Math`
- `Engine/Source/Core/Memory`
- math smoke test 또는 작은 console/sample 검증
- 좌표계/행렬 정책 주석 또는 문서 섹션

완료 조건:

- 큐브 렌더링에 필요한 math 준비
- 프레임 임시 메모리 할당 실험 가능
- projection/view/world matrix를 조합해 예상 위치 변환이 가능

주의할 점:

- SIMD 최적화는 하지 않는다.
- Quaternion은 camera/cube 회전에 필요한 최소 기능만 구현한다.
- MemoryTracker는 완전한 allocator framework가 아니라 누수와 사용량 확인용으로 둔다.

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

세부 작업:

- D3D12 bootstrap 코드를 만든다.
  - debug layer enable
  - DXGI factory 생성
  - hardware adapter 선택
  - device 생성
  - command queue 생성
  - swapchain 생성
  - backbuffer RTV 생성
- 프레임 동기화 구조를 만든다.
  - frame index
  - fence value
  - GPU wait
  - resize 시 GPU idle 처리
- command recording 기본 흐름을 만든다.
  - allocator reset
  - command list reset
  - resource barrier
  - clear render target
  - close/execute
  - present
- resize 처리를 추가한다.
  - backbuffer release
  - swapchain resize
  - RTV 재생성
- D3D12 error helper를 만든다.
  - HRESULT check
  - device removed reason 로그
  - debug name 설정 helper

산출물:

- `Samples/02_D3D12Clear`
- D3D12 bootstrap 코드
- backbuffer clear color 변경 테스트

완료 조건:

- D3D12로 화면 clear
- 창 resize 후에도 clear 유지
- GPU validation/debug layer에서 치명적 경고 없음
- device creation 실패 시 명확한 로그 출력

주의할 점:

- 이 달에는 wrapper를 두껍게 만들지 않는다.
- Raw D3D12 흐름을 이해하고 반복되는 패턴을 기록한다.
- descriptor heap abstraction은 Month 6까지 미룬다.

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

세부 작업:

- Slang 의존성을 External에 배치하고 CMake 연결을 정리한다.
- runtime compile path를 만든다.
  - `.slang` 파일 로드
  - vertex/pixel entry point 지정
  - DXIL target compile
  - compile error 로그 출력
- `Tools/ShaderCompiler` 초안을 만든다.
  - command line input/output
  - target profile
  - entry point
  - `.shaderbin` 파일 출력
  - reflection metadata는 최소 구조만 예약
- triangle 렌더링에 필요한 D3D12 객체를 추가한다.
  - root signature
  - pipeline state
  - input layout
  - vertex buffer upload
  - viewport/scissor
- shader asset 배치를 정한다.
  - `Assets/Source/Shaders`
  - `Assets/Cooked/WindowsD3D12/Shaders`

산출물:

- `Samples/03_SlangTriangle`
- `Tools/ShaderCompiler`
- `Assets/Source/Shaders/Triangle.slang`
- 개발용 runtime compile 경로

완료 조건:

- Slang 셰이더로 삼각형 출력
- shader compile 실패 시 파일명/entry point/error line을 로그로 확인 가능
- runtime compile과 ShaderCompiler CLI가 같은 source를 처리 가능

주의할 점:

- Shipping 경로는 `.shaderbin`만 읽는 방향으로 설계하되, 구현은 개발 편의를 우선한다.
- reflection 기반 자동 root signature는 아직 만들지 않는다.
- root signature는 triangle/cube에 필요한 최소 형태로 둔다.

---

## Month 5: Cube + Depth + Constant Buffer

목표:

- Index buffer
- Constant buffer
- Depth buffer
- MVP matrix
- Camera
- Rotating cube

세부 작업:

- indexed draw 경로를 추가한다.
  - vertex buffer
  - index buffer
  - upload helper
- depth buffer를 만든다.
  - depth texture
  - DSV heap
  - depth clear
  - depth test enabled PSO
- constant buffer 경로를 만든다.
  - per-frame constant buffer
  - 256-byte alignment
  - CPU upload
  - root CBV 또는 descriptor 기반 바인딩 중 단순한 쪽 선택
- Camera를 만든다.
  - position/rotation
  - view matrix
  - projection matrix
  - aspect ratio resize 반영
- cube sample을 만든다.
  - 회전 animation
  - MVP 적용
  - simple vertex color 또는 normal color

산출물:

- `Samples/04_Cube`
- cube vertex/index data
- camera/matrix helper
- depth buffer 생성/해제 코드

완료 조건:

- 회전하는 3D 큐브
- 창 resize 시 projection aspect ratio 정상 반영
- depth test가 동작해 뒷면/앞면 관계가 깨지지 않음

주의할 점:

- material 시스템을 이 달에 만들지 않는다.
- cube sample은 렌더링 foundation 검증용으로 단순하게 유지한다.
- constant buffer update 방식은 나중에 upload ring buffer로 교체될 것을 전제로 둔다.

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

세부 작업:

- Month 3-5에서 반복된 Raw D3D12 코드를 wrapper로 정리한다.
- RHI 방향과 D3D12 backend internal 경계를 나눈다.
  - 상위 API는 descriptor heap handle을 직접 노출하지 않는다.
  - D3D12 전용 세부 구현은 backend 내부로 숨긴다.
- 기본 wrapper를 만든다.
  - `Buffer`
  - `Texture`
  - `Pipeline`
  - `CommandList`
  - `Swapchain`
  - `Fence`
- descriptor allocator를 만든다.
  - RTV/DSV 전용 allocator
  - CBV/SRV/UAV allocator 초안
  - shader visible heap은 단순 ring 또는 고정 heap으로 시작
- upload ring buffer를 만든다.
  - per-frame upload allocation
  - alignment
  - mapped pointer
  - GPU fence 기반 재사용
- frame resource를 정리한다.
  - command allocator
  - upload allocator
  - fence value
  - per-frame temporary resources

산출물:

- `Engine/Source/RHI`
- `Engine/Source/RHI/D3D12`
- wrapper 기반으로 재작성된 cube sample
- D3D12 raw helper 제거 또는 backend 내부 이동

완료 조건:

- Raw D3D12 반복 코드 감소
- Cube sample이 wrapper 기반으로 동작
- clear/triangle/cube sample이 같은 foundation 위에서 유지

주의할 점:

- 완전한 멀티 backend RHI를 만들려고 하지 않는다.
- D3D12를 가리되 explicit API 구조는 유지한다.
- abstraction이 sample 코드를 더 복잡하게 만들면 범위를 줄인다.

---

## Month 7: Asset Cooker 1차

목표:

- Tools/AssetCooker 생성
- Assimp import 연결
- RawModel 정규화
- `.meshbin` writer
- `.shaderbin` writer 기초

세부 작업:

- `Tools/AssetCooker`를 만든다.
  - command line input path
  - output directory
  - target platform: `WindowsD3D12`
  - verbose logging
- Assimp import를 연결한다.
  - glTF/GLB 우선 검증
  - OBJ 보조 검증
  - FBX는 best-effort
- RawModel 중간 표현을 만든다.
  - positions
  - normals
  - tangents, optional
  - uvs
  - indices
  - mesh parts
  - material slot index
- `.meshbin` format v0를 정의한다.
  - magic
  - version
  - endian 가정
  - vertex layout
  - index format
  - submesh table
  - bounds
- shader compile을 AssetCooker 또는 ShaderCompiler와 연결한다.
  - 초기에는 ShaderCompiler 별도 실행 허용
  - 나중에 AssetCooker가 shader dependency를 호출할 수 있게 구조만 준비

산출물:

- `Tools/AssetCooker`
- `.meshbin` writer
- format 문서 또는 코드 주석
- 테스트용 glTF/OBJ 소형 모델

완료 조건:

- Assimp로 읽은 모델을 `.meshbin`으로 변환
- 변환 결과의 vertex/index/submesh 수가 로그로 출력
- 잘못된 input file에 대해 명확히 실패

주의할 점:

- Runtime이 Assimp를 링크하지 않도록 한다.
- format v0는 깨져도 되지만 version 필드를 반드시 둔다.
- skeleton/animation은 범위에서 제외한다.

---

## Month 8: Mesh/Material Runtime

목표:

- `.meshbin` runtime loader
- `.matbin` format
- Material parameter
- Basic PBR shader
- Assimp material slot 변환

세부 작업:

- Runtime mesh loader를 만든다.
  - `.meshbin` header 검증
  - vertex/index buffer 생성
  - submesh 정보 로드
  - bounds 로드
- Asset handle 초안을 만든다.
  - `MeshHandle`
  - `MaterialHandle`
  - load/cache/release 기본 구조
- `.matbin` format v0를 정의한다.
  - baseColor factor
  - metallic factor
  - roughness factor
  - texture reference placeholder
  - shader reference
- Assimp material slot을 `.matbin`으로 변환한다.
  - material name
  - base color
  - roughness/metallic 값
  - texture path는 Month 9에서 실제 처리
- Basic PBR shader를 만든다.
  - directional light 1개
  - baseColor factor
  - roughness/metallic 단순 모델
  - normal map 없음
- cooked mesh sample을 만든다.

산출물:

- `Samples/05_MeshAsset`
- `.meshbin` runtime loader
- `.matbin` writer/loader v0
- Basic PBR shader

완료 조건:

- cooked mesh 렌더링
- submesh/material slot이 최소 1개 이상 정상 반영
- source model 없이 cooked output만으로 sample 실행

주의할 점:

- 이 단계의 PBR은 시각적 정확도보다 material pipeline 검증이 목적이다.
- texture binding은 placeholder로 두고 Month 9에서 구현한다.
- AssetManager는 synchronous load만 지원해도 된다.

---

## Month 9: Texture Pipeline

목표:

- PNG/TGA/DDS/KTX source import
- `.texbin` format
- D3D12 texture upload
- BaseColor texture binding
- Sampler

세부 작업:

- TextureCompiler를 AssetCooker 안에 추가하거나 별도 모듈로 만든다.
  - PNG
  - TGA
  - DDS container import
  - KTX container import
- `.texbin` format v0를 정의한다.
  - magic
  - version
  - width/height/depth
  - mip count
  - format
  - row pitch
  - data offset
- 초기 runtime format은 RGBA8 중심으로 제한한다.
  - 지원하지 않는 DDS/KTX format은 명시적으로 실패
  - BCn 압축은 2년차 이후
- D3D12 texture upload 경로를 만든다.
  - upload buffer
  - `CopyTextureRegion`
  - resource state transition
  - SRV 생성
- material texture binding을 구현한다.
  - baseColor texture
  - default white texture
  - sampler
- shader를 texture sampling 기반으로 갱신한다.

산출물:

- `Samples/06_TextureAsset`
- `.texbin` writer/loader
- baseColor texture가 연결된 `.matbin`
- default texture assets

완료 조건:

- 텍스처가 입혀진 cooked model 출력
- texture 누락 시 default texture로 fallback
- PNG 기반 테스트 asset이 정상 표시

주의할 점:

- mip generation은 optional로 둔다.
- sRGB/linear 정책은 baseColor 중심으로 최소 문서화한다.
- DDS/KTX는 전체 포맷 지원이 아니라 container 처리와 실패 경로 검증이 목표다.

---

## Month 10: ECS 1차

목표:

- Entity generation
- Sparse set component pool
- TransformComponent
- MeshRendererComponent
- CameraComponent
- RenderExtract

세부 작업:

- ECS core를 만든다.
  - `Entity { index, generation }`
  - entity create/destroy
  - generation validation
  - sparse set component pool
- 기본 component를 만든다.
  - `TransformComponent`
  - `MeshRendererComponent`
  - `CameraComponent`
- Transform hierarchy는 초기 범위에서 제외하거나 단일 parent만 문서화한다.
- World update 흐름을 만든다.
  - begin frame
  - update systems
  - render extract
  - end frame
- RenderExtract를 만든다.
  - ECS World에서 renderable 목록 추출
  - transform matrix 계산
  - mesh/material handle 수집
  - renderer는 GameObject를 직접 참조하지 않음
- 여러 entity를 배치하는 sample을 만든다.

산출물:

- `Samples/07_ECS`
- `Engine/Source/World`
- sparse set component pool
- RenderExtract 구조체

완료 조건:

- ECS entity 여러 개 렌더링
- entity destroy 후 stale handle 접근이 검출
- Transform/MeshRenderer/Camera 조합으로 scene 구성 가능

주의할 점:

- archetype ECS는 구현하지 않는다.
- editor-friendly GameObject facade는 얇게만 만든다.
- serialization은 Month 12 또는 2년차 Q2로 미룬다.

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

세부 작업:

- UI command buffer를 만든다.
  - rect
  - text
  - clip rect
  - optional line
- UI renderer를 만든다.
  - orthographic projection
  - dynamic vertex/index buffer
  - alpha blending
  - scissor rect
- bitmap font 기반 text rendering을 만든다.
  - ASCII debug font
  - glyph atlas
  - fixed metrics
- immediate-like debug API를 만든다.
  - `BeginPanel`
  - `EndPanel`
  - `Label`
  - `Button`
  - `SliderFloat`
  - `Checkbox`
- UI input routing 기초를 만든다.
  - pointer position
  - pointer down/up
  - hot/active item
  - keyboard focus는 최소화
- debug panel을 만든다.
  - frame time
  - camera position
  - material parameter
  - reload shader button placeholder

산출물:

- `Samples/08_DebugUI`
- UI command buffer
- bitmap font asset
- basic controls

완료 조건:

- Dear ImGui 없이 디버그 UI 표시
- Button/Slider/Checkbox 입력 가능
- UI pass와 mesh pass가 한 frame에서 함께 렌더링

주의할 점:

- full retained UI tree는 만들지 않는다.
- 텍스트 입력, IME, docking, complex layout은 제외한다.
- Editor UI로 확장 가능한 naming과 layout 단위만 준비한다.

---

## Month 12: Integration Demo

목표:

- Basic scene
- Entity selection 준비
- Camera control
- Material parameter 조정
- 문서화
- v0.1 demo

세부 작업:

- 1년차 기능을 하나의 demo에 통합한다.
  - cooked mesh
  - texture
  - material
  - ECS entity
  - camera
  - debug UI
- camera controller를 정리한다.
  - WASD 이동
  - mouse drag 또는 right-button look
  - speed 조절
- entity selection 준비를 한다.
  - CPU ray/AABB intersection 초안 또는 ID 기반 선택 placeholder
  - 선택된 entity 표시용 debug panel
- material parameter 조정을 UI와 연결한다.
  - baseColor factor
  - roughness
  - metallic
  - texture toggle, optional
- 문서를 정리한다.
  - build 방법
  - asset cook 방법
  - sample 실행 방법
  - format version 상태
  - 알려진 제한
- v0.1 demo tag 기준을 정한다.

산출물:

- `Samples/09_IntegrationDemo` 또는 기존 sample 확장
- `Documentations/07_SamplesWorkflowAndGoals.md` 갱신
- v0.1 demo checklist

완료 조건:

- Windows 11 + D3D12 + Slang + cooked asset + ECS + 자체 UI 데모
- clean checkout 후 build/cook/run 순서가 문서대로 재현
- 1년차 범위에서 다음 해 작업을 막는 큰 구조 부채가 문서화됨

주의할 점:

- 새 기능 추가보다 통합 안정화를 우선한다.
- demo에서만 필요한 임시 코드는 `Samples` 안에 격리한다.
- 2년차 Render Graph로 넘어갈 수 있도록 pass 경계를 기록한다.

---

# 2년차 로드맵

## 2년차 운영 원칙

2년차는 “기능을 늘리는 해”가 아니라 “1년차의 실험 코드를 엔진 구조로 수렴시키는 해”다.

우선순위:

- Render Graph로 렌더링 pass 의존성을 명확히 한다.
- 자체 UI를 Editor foundation으로 확장한다.
- 조명, 그림자, PBR 품질을 개선한다.
- Audio/Physics는 엔진 subsystem의 첫 통합을 목표로 한다.

분기별 산출물 기준:

- 분기마다 하나의 통합 sample을 유지한다.
- asset/runtime format 변경은 cooker와 loader를 같은 분기에 함께 갱신한다.
- Editor 기능은 runtime과 분리하되 같은 UI 시스템 위에서 검증한다.

---

## Quarter 1: Render Graph

목표:

- Pass registration
- Resource read/write declaration
- Backbuffer import
- Depth/resource barrier helper
- UI pass integration

세부 작업:

- Render Graph core를 만든다.
  - pass 등록
  - pass별 read/write resource 선언
  - imported resource
  - transient resource
  - pass execution callback
- resource state tracking을 만든다.
  - texture/buffer state
  - barrier 자동 생성 초안
  - D3D12 transition barrier
- backbuffer/depth를 graph에 import한다.
- 기존 렌더링 흐름을 pass로 나눈다.
  - Clear Pass
  - Mesh Pass
  - UI Pass
  - Present Pass
- frame debugger용 로그를 만든다.
  - pass order
  - resource read/write
  - generated barrier count
- resize 시 graph resource 재생성 경로를 정리한다.

산출물:

- `Samples/09_RenderGraph`
- Render Graph core
- pass/resource debug dump

완료 조건:

- Clear Pass
- Mesh Pass
- UI Pass
- Present Pass
- 기존 integration demo가 Render Graph 기반으로 동작

주의할 점:

- graph compiler를 과하게 만들지 않는다.
- async compute, pass culling, aliasing은 제외한다.
- 먼저 명시적 pass 순서 + barrier helper 수준으로 시작한다.

---

## Quarter 2: Editor Foundation

목표:

- 자체 UI 확장
- Hierarchy panel
- Inspector panel
- Transform editing
- Material editing
- 자체 text scene format

세부 작업:

- Editor application shell을 만든다.
  - viewport
  - hierarchy
  - inspector
  - asset/debug panel placeholder
- UI layout을 확장한다.
  - vertical/horizontal layout
  - scroll area
  - collapsible header
  - selectable row
  - text field 기초, optional
- selection model을 만든다.
  - selected entity
  - hover entity, optional
  - inspector binding
- Transform editing을 구현한다.
  - position
  - rotation
  - scale
  - numeric drag control
- Material editing을 구현한다.
  - baseColor
  - roughness
  - metallic
  - texture reference 표시
- 자체 text scene format을 만든다.
  - entity
  - transform
  - mesh
  - material
  - camera
- scene save/load 초기 버전을 만든다.

산출물:

- `Tools/Editor` 또는 `Samples/10_EditorFoundation`
- scene text format parser/writer
- hierarchy/inspector UI

완료 조건:

- Entity 선택
- Transform 수정
- Material parameter 수정
- Scene 저장/로드 초기 버전

주의할 점:

- YAML 전환은 나중으로 둔다.
- gizmo는 optional로 두고 numeric editing부터 완성한다.
- Editor와 Runtime이 같은 cooked asset loader를 사용하도록 한다.

---

## Quarter 3: Lighting / Shadow / PBR 개선

목표:

- Directional light
- Point light 기초
- Shadow map
- Normal map
- Metallic/roughness 개선
- IBL 준비

세부 작업:

- LightComponent를 추가한다.
  - directional light
  - point light
  - color/intensity
  - range
- renderer의 light data upload 경로를 만든다.
  - per-frame light buffer
  - 최대 light count 제한
  - shader binding
- shadow map pass를 만든다.
  - directional light shadow
  - depth-only pipeline
  - shadow atlas는 후순위
  - simple PCF
- normal map을 material/texture pipeline에 추가한다.
  - tangent import
  - tangent basis 검증
  - normal texture binding
- PBR shader를 개선한다.
  - metallic/roughness BRDF 정리
  - energy conservation 기본
  - gamma/sRGB 처리 검증
- IBL 준비만 한다.
  - environment texture format 검토
  - irradiance/prefilter는 다음 단계 후보로 문서화

산출물:

- lighting sample
- shadow map pass
- normal map material
- PBR shader v1

완료 조건:

- textured mesh에 조명과 그림자 적용
- normal map 유무 차이를 시각적으로 확인 가능
- material parameter 변경이 lighting 결과에 반영

주의할 점:

- Forward+는 아직 구현하지 않는다.
- shadow 품질보다 pass 구조와 material 확장이 목적이다.
- IBL은 실제 구현보다 format/API 준비에 그친다.

---

## Quarter 4: Audio + Physics 1차

목표:

- WASAPI shared backend
- WAV PCM loader
- Opus decoder
- 자체 mixer
- Jolt Physics 통합
- ECS PhysicsBodyComponent

세부 작업:

- Audio subsystem 초안을 만든다.
  - audio device init
  - WASAPI shared mode
  - output format negotiation
  - audio thread
  - ring buffer
- WAV PCM loader를 만든다.
  - RIFF parsing
  - PCM16/float support 중 우선순위 선택
  - sample rate/channel conversion은 최소화
- Opus decoder를 연결한다.
  - Tools 또는 Runtime 사용 범위 결정
  - streaming decode는 후순위
- 자체 mixer를 만든다.
  - sound handle
  - play/stop
  - volume
  - loop
  - BGM/effect 구분
- Jolt Physics를 통합한다.
  - physics world
  - static body
  - dynamic rigid body
  - box/sphere collider
  - simulation step
- ECS와 physics sync를 만든다.
  - `PhysicsBodyComponent`
  - transform to physics
  - physics to transform
  - fixed timestep 정책

산출물:

- `Samples/10_Audio`
- `Samples/11_Physics`
- Audio subsystem
- Physics subsystem
- ECS sync sample

완료 조건:

- 효과음/BGM 재생
- 기본 rigid body 시뮬레이션
- ECS와 physics sync

주의할 점:

- 3D spatial audio는 후순위다.
- physics editor tooling은 제외한다.
- audio streaming과 DirectStorage 연동은 3년차 이후로 둔다.

---

# 3년차 로드맵

## 3년차 운영 원칙

3년차는 package, streaming, platform expansion을 통해 엔진의 장기 구조를 검증하는 해다.

우선순위:

- cooked asset을 package 단위로 묶는다.
- async loading과 placeholder asset 구조를 만든다.
- DirectStorage는 optional capability로 통합한다.
- D3D12 중심 RHI가 다른 explicit backend를 수용할 수 있는지 검증한다.

---

## Quarter 1: Package Format

목표:

- `.pak` format
- AssetTable
- ChunkTable
- DependencyTable
- BulkData
- cooked scene binary

세부 작업:

- package format v0를 정의한다.
  - magic
  - version
  - platform
  - table offsets
  - alignment
  - checksum, optional
- AssetTable을 만든다.
  - asset id
  - asset type
  - dependency range
  - chunk reference
- ChunkTable을 만든다.
  - offset
  - compressed size
  - uncompressed size
  - compression type
  - target memory hint
- DependencyTable을 만든다.
  - mesh -> material
  - material -> texture/shader
  - scene -> entity assets
- BulkData 영역을 만든다.
  - buffer payload
  - texture payload
  - alignment policy
- PackageBuilder를 만든다.
  - cooked assets 입력
  - `.pak` 출력
  - manifest 출력
- cooked scene binary를 만든다.
  - text scene -> `.scenebin`
  - `.scenebin` -> package

산출물:

- `Tools/PackageBuilder`
- `.pak` reader
- `.scenebin` writer/loader
- package manifest

완료 조건:

- 여러 cooked asset을 하나의 package에서 로드
- 기존 sample이 loose cooked asset 또는 package 중 하나로 실행 가능
- asset dependency 누락 시 명확한 오류 출력

주의할 점:

- compression은 none부터 시작한다.
- patching, encryption, DLC는 범위에서 제외한다.
- package reader는 streaming을 고려한 offset 기반 API로 만든다.

---

## Quarter 2: Async Streaming

목표:

- Storage abstraction
- Background loading
- Request queue
- Completion queue
- CPU fallback upload path

세부 작업:

- Storage abstraction을 만든다.
  - file read
  - package read
  - async request interface
  - sync fallback
- background loading thread를 만든다.
  - request queue
  - completion queue
  - cancellation 정책 초안
  - shutdown 처리
- AssetManager를 async load 대응으로 확장한다.
  - pending handle
  - loaded handle
  - failed handle
  - reference count 또는 lifetime policy
- placeholder asset을 만든다.
  - default mesh
  - default material
  - default texture
- GPU upload를 분리한다.
  - background thread는 CPU data read/decode
  - render thread는 GPU resource create/upload
  - upload completion tracking
- loading 상태를 UI/debug panel에서 확인한다.

산출물:

- async storage layer
- async asset loading sample
- placeholder asset path
- loading debug panel

완료 조건:

- 로딩 중 프레임 유지
- placeholder asset 사용 가능
- asset load 완료 후 실제 resource로 교체

주의할 점:

- 모든 asset을 async로 바꾸려고 하지 않는다.
- scene streaming보다 단일 asset async load부터 검증한다.
- thread safety 범위를 문서화한다.

---

## Quarter 3: DirectStorage 1차

목표:

- DirectStorage initialization
- Package file open
- Uncompressed chunk -> GPU buffer
- Fence/completion integration
- CPU fallback

세부 작업:

- DirectStorage capability detection을 만든다.
  - supported 여부
  - fallback 사유 로그
  - optional dependency 처리
- DirectStorage factory/queue를 초기화한다.
  - file source
  - memory destination
  - GPU buffer destination
  - completion event/fence
- package chunk를 DirectStorage request로 연결한다.
  - chunk offset
  - size
  - destination buffer
  - alignment 확인
- uncompressed chunk부터 GPU buffer로 업로드한다.
  - mesh vertex buffer
  - index buffer
  - CPU fallback 결과와 비교
- completion integration을 만든다.
  - request id
  - status check
  - error handling
  - render thread handoff
- fallback path를 유지한다.
  - DirectStorage unavailable
  - request failure
  - unsupported chunk type

산출물:

- DirectStorage backend
- CPU storage backend
- backend comparison sample
- diagnostics log

완료 조건:

- DirectStorage 경로와 CPU fallback 경로 결과 동일
- DirectStorage 미지원 환경에서도 sample 실행 가능
- 실패 시 fallback 여부를 로그로 확인 가능

주의할 점:

- GPU decompression은 후순위다.
- texture direct upload는 buffer path가 안정된 뒤 진행한다.
- DirectStorage는 필수 기능이 아니라 optional acceleration이다.

---

## Quarter 4: Platform Expansion / Vulkan Preparation

목표:

- Vulkan backend prototype on Windows
- RHI 검증
- Shader target abstraction 개선
- WebGPU/macOS/Linux 장기 준비 문서화

세부 작업:

- RHI API를 점검한다.
  - D3D12 descriptor heap 누출 여부
  - resource state abstraction
  - bind group 모델
  - pipeline layout 모델
  - swapchain abstraction
- Vulkan backend prototype을 만든다.
  - instance/device/swapchain
  - command buffer
  - render pass 또는 dynamic rendering
  - buffer/texture
  - triangle 또는 clear sample
- Shader target abstraction을 개선한다.
  - Slang target 분리
  - DXIL/SPIR-V output
  - reflection metadata 공통화 검토
- platform layer 장기 계획을 문서화한다.
  - macOS + Metal
  - Linux + Vulkan
  - WebGPU
  - input/audio 차이
- 기존 D3D12 backend와 충돌하는 abstraction을 정리한다.

산출물:

- Vulkan clear 또는 triangle prototype
- RHI gap report
- shader target abstraction 문서
- platform expansion plan

완료 조건:

- D3D12 중심 RHI가 다른 explicit backend에도 대응 가능한지 검증
- 최소 1개 Vulkan prototype sample 실행
- RHI 수정 필요 항목이 backlog로 정리

주의할 점:

- Vulkan backend 완성은 목표가 아니다.
- Windows에서 RHI 검증용으로만 시작한다.
- Metal/WebGPU 구현은 문서화와 API 준비까지만 한다.

---

# 장기 Backlog

## Rendering

- Forward+ light culling
- IBL
- postprocess
- GPU scene
- indirect draw
- compute culling
- meshlet/cluster renderer
- bindless-style resource indexing
- async compute
- ray tracing, optional

## Asset / Tools

- BC1/BC3/BC5/BC7 texture compression
- mip generation
- skeletal mesh
- animation clip
- animation graph
- asset dependency viewer
- package compression
- incremental cooking
- hot reload

## Editor

- transform gizmo
- viewport picking
- asset browser
- scene hierarchy drag/drop
- undo/redo
- prefab-like template
- editor shortcuts
- docking layout

## UI

- retained UI tree
- layout cache
- FreeType glyph atlas
- UTF-8 text
- HarfBuzz shaping
- fallback font
- IME composition
- runtime UI theme

## Audio / Physics

- 3D spatial audio
- streaming music
- audio asset cooking
- physics debug draw
- character controller
- trigger/collision event
- physics material

## Platform

- XInput gamepad complete path
- Raw Input mouse
- GameInput 검토
- touchpad gesture
- macOS platform layer
- Linux platform layer
- WebGPU backend

---

# 릴리스 기준

## v0.1

기준:

- Windows 11
- D3D12
- Slang shader
- cooked mesh/material/texture
- ECS entity rendering
- 자체 debug UI
- build/cook/run 문서화

릴리스 성격:

- 외부 사용 목적이 아닌 내부 milestone
- API 안정성 보장 없음
- format version 변경 가능

## v0.2

기준:

- Render Graph
- Editor foundation
- scene save/load
- lighting/shadow 개선
- Audio/Physics 1차

릴리스 성격:

- 작은 scene을 editor에서 구성하고 실행 가능
- runtime/editor 경계 검증

## v0.3

기준:

- package format
- async asset loading
- DirectStorage optional path
- RHI portability 검증

릴리스 성격:

- loose file 중심에서 package 중심 runtime으로 전환
- 장기 platform expansion 가능성 검증

---

# 리스크와 대응

## 개발 시간 부족

리스크:

- 하루 1시간 기준으로 월별 목표가 밀릴 수 있다.

대응:

- 월별 완료 조건을 sample 중심으로 유지한다.
- optional 항목은 다음 달로 넘긴다.
- 매월 마지막 주는 통합/정리 전용으로 둔다.

## D3D12 복잡도

리스크:

- descriptor, barrier, synchronization에서 디버깅 시간이 길어질 수 있다.

대응:

- debug layer와 GPU validation을 초기에 켠다.
- sample을 작게 유지한다.
- wrapper 도입은 Raw D3D12 흐름을 충분히 반복한 뒤 진행한다.

## Asset Pipeline 범위 증가

리스크:

- format, importer, material 변환 범위가 빠르게 커질 수 있다.

대응:

- glTF/GLB 우선으로 검증한다.
- FBX는 best-effort로 둔다.
- Runtime은 cooked asset만 읽는 원칙을 유지한다.

## UI/Editor 장기화

리스크:

- 자체 UI와 editor가 별도 프로젝트 수준으로 커질 수 있다.

대응:

- debug UI -> editor panel -> retained UI 순서로 확장한다.
- text input, IME, docking은 후순위로 둔다.
- editor 기능은 scene 구성에 필요한 최소 기능부터 구현한다.

## Platform 확장 조기 착수

리스크:

- D3D12도 안정되기 전에 Vulkan/Metal/WebGPU까지 고려하면 구조가 과해진다.

대응:

- 1년차와 2년차는 Windows + D3D12를 기준으로 한다.
- RHI는 explicit API 친화적으로만 유지한다.
- 실제 Vulkan prototype은 3년차 Q4까지 미룬다.
