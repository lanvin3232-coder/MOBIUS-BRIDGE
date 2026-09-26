#ifndef MOBIUS_BRIDGE_H
#define MOBIUS_BRIDGE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mb_ctx mb_ctx;

int mb_create(mb_ctx **ctx);

int mb_run(
    mb_ctx *ctx,
    const void *input,
    size_t input_len,
    void *output,
    size_t *output_len
);

void mb_destroy(mb_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif
