#include "mobius_bridge.h"

#include <stdio.h>
#include <string.h>

static int test_normal_flow(void) {
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
    return 0;
}

static int test_null_input_rejected(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 10;
    }

    char output[16] = {0};
    size_t output_len = sizeof(output);

    int rc = mb_run(
        ctx,
        NULL,
        4,
        output,
        &output_len
    );

    mb_destroy(ctx);

    if (rc != -3) {
        return 11;
    }

    return 0;
}

static int test_size_negotiation(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 20;
    }

    const char input[] = "12345678";

    size_t output_len = 0;

    int rc = mb_run(
        ctx,
        input,
        sizeof(input),
        NULL,
        &output_len
    );

    mb_destroy(ctx);

    if (rc != 1) {
        return 21;
    }

    if (output_len != sizeof(input)) {
        return 22;
    }

    return 0;
}

int main(void) {
    int rc;

    rc = test_normal_flow();
    if (rc != 0) {
        return rc;
    }

    rc = test_null_input_rejected();
    if (rc != 0) {
        return rc;
    }

    rc = test_size_negotiation();
    if (rc != 0) {
        return rc;
    }

    printf("MOBIUS-BRIDGE tests: PASS\n");
    return 0;
}