#ifndef MOBIUS_BRIDGE_H
#define MOBIUS_BRIDGE_H

#include <stddef.h>

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(MOBIUS_BRIDGE_BUILD)
    #define MB_API __declspec(dllexport)
  #else
    #define MB_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && (__GNUC__ >= 4)
  #define MB_API __attribute__((visibility("default")))
#else
  #define MB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MB_ABI_VERSION_MAJOR 0
#define MB_ABI_VERSION_MINOR 1
#define MB_ABI_VERSION_PATCH 0

typedef enum mb_status {
    MB_OK = 0,
    MB_NEED_OUTPUT = 1,

    MB_ERR_INVALID = -1,
    MB_ERR_NOMEM = -2,
    MB_ERR_NULL_INPUT = -3
} mb_status;

typedef struct mb_ctx mb_ctx;

MB_API int mb_create(mb_ctx **ctx);

MB_API int mb_run(
    mb_ctx *ctx,
    const void *input,
    size_t input_len,
    void *output,
    size_t *output_len
);

MB_API void mb_destroy(mb_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif