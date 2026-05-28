#pragma once

#if defined(_WIN32)
#define SIMPLEGL_DLL_EXPORT __declspec(dllexport)
#define SIMPLEGL_DLL_IMPORT __declspec(dllimport)
#define SIMPLEGL_API_CALL __cdecl
#elif defined(__GNUC__) || defined(__clang__)
#define SIMPLEGL_DLL_EXPORT __attribute__((visibility("default")))
#define SIMPLEGL_DLL_IMPORT __attribute__((visibility("default")))
#define SIMPLEGL_API_CALL
#else
#error "Unknown compiler!"
#endif

#ifdef SIMPLEGL_CORE
#define SIMPLEGL_API SIMPLEGL_DLL_EXPORT SIMPLEGL_API_CALL
#else
#define SIMPLEGL_API SIMPLEGL_DLL_IMPORT SIMPLEGL_API_CALL
#endif
