#include "mobius_bridge.h"

#include <stdlib.h>
#include <string.h>

struct mb_ctx {
    int ready;
};

int mb_create(mb_ctx **ctx) {
    if (!ctx) {
        return -1;
    }

    *ctx = (mb_ctx *)malloc(sizeof(mb_ctx));
    if (!*ctx) {
        return -2;
    }

    (*ctx)->ready = 1;
    return 0;
}

int mb_run(
    mb_ctx *ctx,
    const void *input,
    size_t input_len,
    void *output,
    size_t *output_len
) {
    if (!ctx || !ctx->ready || !output_len) {
        return -1;
    }

    if (input_len > 0 && !input) {
        return -3;
    }

    if (input_len == 0) {
        *output_len = 0;
        return 0;
    }

    if (!output || *output_len < input_len) {
        *output_len = input_len;
        return 1;
    }

    memcpy(output, input, input_len);

    *output_len = input_len;
    return 0;
}

void mb_destroy(mb_ctx *ctx) {
    if (!ctx) {
        return;
    }

    ctx->ready = 0;
    free(ctx);
}