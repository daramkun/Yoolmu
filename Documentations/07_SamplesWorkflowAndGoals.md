# 샘플 정책

모든 기능은 샘플로 검증한다.

```text
Samples/
  00_Window
  01_Input
  02_D3D12Clear
  03_SlangTriangle
  04_Cube
  05_MeshAsset
  06_TextureAsset
  07_ECS
  08_DebugUI
  09_RenderGraph
  10_Audio
  11_Physics
  12_Package
  13_DirectStorage
```

샘플이 없으면 기능이 완성된 것이 아니다.

---

# 하루 1시간 작업 원칙

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

# 매주 / 매월 점검

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

# 최종 목표 문장

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
