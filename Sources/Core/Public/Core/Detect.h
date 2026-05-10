#pragma once

#if __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunused-macros"
#endif

//////////////////////////////////////////////////////////////
// Compiler Detections
//////////////////////////////////////////////////////////////

#if defined(_MSC_VER) || _MSC_VER
#   define COMPILER_MSVC                                     1
#else
#   define COMPILER_MSVC                                     0
#endif
#if defined(__GNUC__) && !defined(__clang__) || (__GNUC__ && !__clang__)
#   define COMPILER_GCC                                      1
#else
#   define COMPILER_GCC                                      0
#endif
#if defined(__clang__) || __clang__
#   define COMPILER_CLANG                                    1
#else
#   define COMPILER_CLANG                                    0
#endif

//////////////////////////////////////////////////////////////
// OS Detections
//////////////////////////////////////////////////////////////

#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#	include <Windows.h>
#   if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_APP
#       define PLATFORM_WINDOWS                              0
#	    define PLATFORM_UWP                                  1
#   elif !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#	    define PLATFORM_WINDOWS                              1
#	    define PLATFORM_UWP                                  0
#   endif
#else
#	define PLATFORM_WINDOWS                                  0
#	define PLATFORM_UWP                                      0
#endif
#if defined(__ANDROID__) || __ANDROID__
#   define PLATFORM_ANDROID                                  1
#else
#   define PLATFORM_ANDROID                                  0
#endif
#if (defined(__linux__) || __linux__) && !PLATFORM_ANDROID
#   define PLATFORM_LINUX                                    1
#else
#   define PLATFORM_LINUX                                    0
#endif
#if defined (__APPLE__)
#	include <TargetConditionals.h>
#   if TARGET_OS_IOS
#       define PLATFORM_MACOS                                0
#       define PLATFORM_IOS                                  1
#       define PLATFORM_TVOS                                 0
#       define PLATFORM_WATCHOS                              0
#       define PLATFORM_VISIONOS                             0
#   elif TARGET_OS_TVOS
#       define PLATFORM_MACOS                                0
#       define PLATFORM_IOS                                  0
#       define PLATFORM_TVOS                                 1
#       define PLATFORM_WATCHOS                              0
#       define PLATFORM_VISIONOS                             0
#   elif TARGET_OS_WATCHOS
#       define PLATFORM_MACOS                                0
#       define PLATFORM_IOS                                  0
#       define PLATFORM_TVOS                                 0
#       define PLATFORM_WATCHOS                              1
#       define PLATFORM_VISIONOS                             0
#   elif TARGET_OS_VISIONOS
#       define PLATFORM_MACOS                                0
#       define PLATFORM_IOS                                  0
#       define PLATFORM_TVOS                                 0
#       define PLATFORM_WATCHOS                              0
#       define PLATFORM_VISIONOS                             1
#   else
#       define PLATFORM_MACOS                                1
#       define PLATFORM_IOS                                  0
#       define PLATFORM_TVOS                                 0
#       define PLATFORM_WATCHOS                              0
#       define PLATFORM_VISIONOS                             0
#   endif
#else
#   define PLATFORM_MACOS                                    0
#   define PLATFORM_IOS                                      0
#   define PLATFORM_TVOS                                     0
#   define PLATFORM_WATCHOS                                  0
#   define PLATFORM_VISIONOS                                 0
#endif
#if defined(__EMSCRIPTEN__) || defined(__asmjs__) || __EMSCRIPTEN__ || __asmjs__
#   define PLATFORM_WEBASSEMBLY                              1
#else
#   define PLATFORM_WEBASSEMBLY                              0
#endif

//////////////////////////////////////////////////////////////
// CPU Architecture Detections
//////////////////////////////////////////////////////////////

#if defined(_M_AMD64) || _M_AMD64 || defined(__amd64__) || __amd64__
#   define ARCH_AMD64                                        1
#else
#   define ARCH_AMD64                                        0
#endif
#if defined(_M_ARM64) || _M_ARM64 || defined(__aarch64__) || __aarch64__
#   define ARCH_ARM64                                        1
#else
#   define ARCH_ARM64                                        0
#endif

#if __clang__
#   pragma clang diagnostic pop
#endif
