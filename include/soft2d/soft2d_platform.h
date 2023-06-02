#pragma once

// TO KEEP THE INCLUDE DEPENDENCY CLEAN, PLEASE DO NOT INCLUDE ANY OTHER
// TAICHI HEADERS INTO THIS ONE.
//
// TODO(#2196): Once we can slim down "taichi/common/core.h", consider moving
// the contents back to core.h and delete this file.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifdef __GNUC__
#define S2_API __attribute__((dllexport))
#define S2_API_CALL
#else
#define S2_API __declspec(dllexport)
#define S2_API_CALL __stdcall
#endif //  __GNUC__
#else
#define S2_API __attribute__((visibility("default")))
#define S2_API_CALL
#endif // defined _WIN32 || defined _WIN64 || defined __CYGWIN__

// Windows
#if defined(_WIN64)
#define S2_PLATFORM_WINDOWS
#endif

#if defined(_WIN32) && !defined(_WIN64)
static_assert(false, "32-bit Windows systems are not supported")
#endif

// Linux
#if defined(__linux__)
#if defined(ANDROID)
#define S2_PLATFORM_ANDROID
#else
#define S2_PLATFORM_LINUX
#endif
#endif

// OSX
#if defined(__APPLE__)
#define S2_PLATFORM_OSX
#endif

#if (defined(S2_PLATFORM_LINUX) || defined(S2_PLATFORM_OSX) ||                 \
     defined(__unix__))
#define S2_PLATFORM_UNIX
#endif

#include <stddef.h>
#include <stdint.h>
