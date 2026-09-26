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

static int test_create_null_rejected(void) {
    if (mb_create(NULL) != -1) {
        return 10;
    }

    return 0;
}

static int test_null_context_rejected(void) {
    const char input[] = "x";
    char output[8] = {0};
    size_t output_len = sizeof(output);

    if (mb_run(
        NULL,
        input,
        sizeof(input),
        output,
        &output_len
    ) != -1) {
        return 20;
    }

    return 0;
}

static int test_null_output_length_rejected(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 30;
    }

    const char input[] = "x";
    char output[8] = {0};

    int rc = mb_run(
        ctx,
        input,
        sizeof(input),
        output,
        NULL
    );

    mb_destroy(ctx);

    if (rc != -1) {
        return 31;
    }

    return 0;
}

static int test_null_input_rejected(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 40;
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
        return 41;
    }

    return 0;
}

static int test_size_negotiation(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 50;
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
        return 51;
    }

    if (output_len != sizeof(input)) {
        return 52;
    }

    return 0;
}

static int test_zero_length_input(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != 0) {
        return 60;
    }

    size_t output_len = 0;

    int rc = mb_run(
        ctx,
        NULL,
        0,
        NULL,
        &output_len
    );

    mb_destroy(ctx);

    if (rc != 0) {
        return 61;
    }

    if (output_len != 0) {
        return 62;
    }

    return 0;
}

int main(void) {
    int rc;

    rc = test_normal_flow();
    if (rc != 0) return rc;

    rc = test_create_null_rejected();
    if (rc != 0) return rc;

    rc = test_null_context_rejected();
    if (rc != 0) return rc;

    rc = test_null_output_length_rejected();
    if (rc != 0) return rc;

    rc = test_null_input_rejected();
    if (rc != 0) return rc;

    rc = test_size_negotiation();
    if (rc != 0) return rc;

    rc = test_zero_length_input();
    if (rc != 0) return rc;

    printf("MOBIUS-BRIDGE boundary tests: PASS\n");
    return 0;
}