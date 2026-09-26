#include "mobius_bridge.h"

#include <stdio.h>
#include <string.h>

_Static_assert(MB_ABI_VERSION_MAJOR == 0, "ABI major changed");
_Static_assert(MB_ABI_VERSION_MINOR == 1, "ABI minor changed");
_Static_assert(MB_ABI_VERSION_PATCH == 0, "ABI patch changed");

_Static_assert(MB_OK == 0, "MB_OK value changed");
_Static_assert(MB_NEED_OUTPUT == 1, "MB_NEED_OUTPUT value changed");
_Static_assert(MB_ERR_INVALID == -1, "MB_ERR_INVALID value changed");
_Static_assert(MB_ERR_NOMEM == -2, "MB_ERR_NOMEM value changed");
_Static_assert(MB_ERR_NULL_INPUT == -3, "MB_ERR_NULL_INPUT value changed");

static int test_normal_flow(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != MB_OK) {
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
    ) != MB_OK) {
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
    if (mb_create(NULL) != MB_ERR_INVALID) {
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
    ) != MB_ERR_INVALID) {
        return 20;
    }

    return 0;
}

static int test_null_output_length_rejected(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != MB_OK) {
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

    if (rc != MB_ERR_INVALID) {
        return 31;
    }

    return 0;
}

static int test_null_input_rejected(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != MB_OK) {
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

    if (rc != MB_ERR_NULL_INPUT) {
        return 41;
    }

    return 0;
}

static int test_size_negotiation(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != MB_OK) {
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

    if (rc != MB_NEED_OUTPUT) {
        return 51;
    }

    if (output_len != sizeof(input)) {
        return 52;
    }

    return 0;
}

static int test_zero_length_input(void) {
    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != MB_OK) {
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

    if (rc != MB_OK) {
        return 61;
    }

    if (output_len != 0) {
        return 62;
    }

    return 0;
}

static int test_boundary_sweep(void) {
    mb_ctx *ctx = NULL;

    unsigned char input[4096];
    unsigned char output[4112];

    for (size_t i = 0; i < sizeof(input); ++i) {
        input[i] = (unsigned char)(i & 0xffu);
    }

    if (mb_create(&ctx) != MB_OK) {
        return 70;
    }

    for (size_t len = 1; len <= sizeof(input); ++len) {
        size_t output_len = 0;

        int rc = mb_run(
            ctx,
            input,
            len,
            NULL,
            &output_len
        );

        if (rc != MB_NEED_OUTPUT) {
            mb_destroy(ctx);
            return 71;
        }

        if (output_len != len) {
            mb_destroy(ctx);
            return 72;
        }

        memset(output, 0xA5, sizeof(output));

        output_len = len - 1;

        rc = mb_run(
            ctx,
            input,
            len,
            output,
            &output_len
        );

        if (rc != MB_NEED_OUTPUT) {
            mb_destroy(ctx);
            return 73;
        }

        if (output_len != len) {
            mb_destroy(ctx);
            return 74;
        }

        output_len = len;

        rc = mb_run(
            ctx,
            input,
            len,
            output,
            &output_len
        );

        if (rc != MB_OK) {
            mb_destroy(ctx);
            return 75;
        }

        if (output_len != len) {
            mb_destroy(ctx);
            return 76;
        }

        if (memcmp(input, output, len) != 0) {
            mb_destroy(ctx);
            return 77;
        }

        for (size_t i = len; i < sizeof(output); ++i) {
            if (output[i] != 0xA5) {
                mb_destroy(ctx);
                return 78;
            }
        }
    }

    mb_destroy(ctx);
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

    rc = test_boundary_sweep();
    if (rc != 0) return rc;

    printf("MOBIUS-BRIDGE ABI + boundary sweep: PASS\n");
    return 0;
}