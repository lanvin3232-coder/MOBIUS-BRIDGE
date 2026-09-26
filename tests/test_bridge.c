#include "mobius_bridge.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 1;
    }

    const char input[] = "mobius-test";
    char output[64] = {0};
    size_t output_len = sizeof(output);

    if (mb_run(
        ctx,
        input,
        strlen(input) + 1,
        output,
        &output_len
    ) != 0) {
        mb_destroy(ctx);
        return 2;
    }

    if (strcmp(input, output) != 0) {
        mb_destroy(ctx);
        return 3;
    }

    mb_destroy(ctx);

    printf("MOBIUS-BRIDGE test: PASS\n");
    return 0;
}
