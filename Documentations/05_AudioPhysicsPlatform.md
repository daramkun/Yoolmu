# Audio 정책

## 기본 정책

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

## 엔진 오디오 구조

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

## 내부 포맷

```text
Sample format:
  float32

Recommended default:
  48 kHz
  stereo
  interleaved float32
```

초기에는 interleaved float32를 사용한다.

## 초기 범위

- WASAPI shared mode
- WAV PCM loader
- Opus decoder
- AudioSource
- AudioVoice
- AudioBus
- Master/SFX/BGM volume
- looping
- play/stop

## Steam Audio

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

# Physics 정책

## 기본 정책

물리 엔진은 직접 구현하지 않는다.

1차 후보는 Jolt Physics다.

## ECS 연동

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

## 장기 검토

Fully-parallel physics engine 후보는 장기적으로 비교한다.

단, 초기에는 Jolt Physics 통합을 우선한다.

---

# Scripting 정책

초기에는 스크립팅을 넣지 않는다.

```text
Initial:
  C++ only

Later:
  Lua or C# 검토 가능
```

스크립팅은 범위 폭발 위험이 있으므로 후순위로 둔다.

---

# Build System

## 초기

1년차는 CMake로 고정한다.

```text
Build System:
  CMake
```

## Dotori

Dotori는 2년차 이후 검토한다.

```text
1년차:
  CMake

2년차 이후:
  Dotori 도입 검토
```

Dotori를 도입하더라도 CMake fallback 또는 export 경로를 유지한다.

---

# DirectStorage 정책

## 기본 정책

DirectStorage는 package/streaming 구조가 안정된 뒤 도입한다.

1차 목표:

```text
uncompressed package chunk → GPU buffer
```

## 단계

```text
1. Package format
2. CPU async streaming
3. Storage abstraction
4. DirectStorage uncompressed buffer path
5. GDeflate
6. Texture mip streaming
```

## Fallback

CPU fallback은 반드시 유지한다.

```text
DirectStorage available:
  package chunk → DirectStorage → GPU buffer

Fallback:
  package chunk → CPU read → upload buffer → GPU buffer
```

---

# Web / Linux 정책

## Web

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

## Linux

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
