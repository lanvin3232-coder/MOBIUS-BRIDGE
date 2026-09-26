#include "mobius_bridge.h"

#include <stdlib.h>
#include <string.h>

struct mb_ctx {
    int ready;
};

int mb_create(mb_ctx **ctx) {
    if (!ctx) {
        return MB_ERR_INVALID;
    }

    *ctx = (mb_ctx *)malloc(sizeof(mb_ctx));
    if (!*ctx) {
        return MB_ERR_NOMEM;
    }

    (*ctx)->ready = 1;
    return MB_OK;
}

int mb_run(
    mb_ctx *ctx,
    const void *input,
    size_t input_len,
    void *output,
    size_t *output_len
) {
    if (!ctx || !ctx->ready || !output_len) {
        return MB_ERR_INVALID;
    }

    if (input_len > 0 && !input) {
        return MB_ERR_NULL_INPUT;
    }

    if (input_len == 0) {
        *output_len = 0;
        return MB_OK;
    }

    if (!output || *output_len < input_len) {
        *output_len = input_len;
        return MB_NEED_OUTPUT;
    }

    memmove(output, input, input_len);

    *output_len = input_len;
    return MB_OK;
}

void mb_destroy(mb_ctx *ctx) {
    if (!ctx) {
        return;
    }

    ctx->ready = 0;
    free(ctx);
}