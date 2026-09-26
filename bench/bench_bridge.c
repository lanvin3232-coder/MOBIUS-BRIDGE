#define _POSIX_C_SOURCE 200809L

#include "mobius_bridge.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BENCH_MAX_SIZE 4096u
#define BENCH_WARMUP 10000u
#define BENCH_ITERATIONS 200000u

static volatile uint64_t bench_sink = 0;

static uint64_t now_ns(void) {
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return ((uint64_t)ts.tv_sec * 1000000000ull)
        + (uint64_t)ts.tv_nsec;
}

static void fill_input(
    unsigned char *buffer,
    size_t len
) {
    for (size_t i = 0; i < len; ++i) {
        buffer[i] = (unsigned char)(
            ((i * 131u) + 17u) & 0xffu
        );
    }
}

static int run_case(
    mb_ctx *ctx,
    size_t payload_size
) {
    unsigned char input[BENCH_MAX_SIZE];
    unsigned char output[BENCH_MAX_SIZE];

    if (payload_size > sizeof(input)) {
        return 10;
    }

    fill_input(input, payload_size);
    memset(output, 0, sizeof(output));

    for (size_t i = 0; i < BENCH_WARMUP; ++i) {
        size_t output_len = payload_size;

        int rc = mb_run(
            ctx,
            input,
            payload_size,
            output,
            &output_len
        );

        if (rc != MB_OK) {
            return 11;
        }

        if (output_len != payload_size) {
            return 12;
        }
    }

    uint64_t start = now_ns();

    if (start == 0) {
        return 13;
    }

    for (size_t i = 0; i < BENCH_ITERATIONS; ++i) {
        size_t output_len = payload_size;

        int rc = mb_run(
            ctx,
            input,
            payload_size,
            output,
            &output_len
        );

        if (rc != MB_OK) {
            return 14;
        }

        if (output_len != payload_size) {
            return 15;
        }

        if (payload_size > 0) {
            bench_sink += output[
                i % payload_size
            ];
        }
    }

    uint64_t end = now_ns();

    if (end == 0 || end <= start) {
        return 16;
    }

    uint64_t elapsed_ns = end - start;

    double ns_per_call =
        (double)elapsed_ns
        / (double)BENCH_ITERATIONS;

    double calls_per_second =
        1000000000.0 / ns_per_call;

    double mib_per_second = 0.0;

    if (payload_size > 0) {
        mib_per_second =
            (
                (double)payload_size
                * calls_per_second
            )
            / (1024.0 * 1024.0);
    }

    printf(
        "%zu,%.3f,%.3f,%.3f\n",
        payload_size,
        ns_per_call,
        calls_per_second,
        mib_per_second
    );

    return 0;
}

int main(void) {
    static const size_t payload_sizes[] = {
        0u,
        1u,
        8u,
        64u,
        256u,
        1024u,
        4096u
    };

    mb_ctx *ctx = NULL;

    if (mb_create(&ctx) != MB_OK) {
        fprintf(
            stderr,
            "benchmark: mb_create failed\n"
        );
        return 1;
    }

    printf(
        "bytes,ns_per_call,calls_per_second,MiB_per_second\n"
    );

    for (
        size_t i = 0;
        i < sizeof(payload_sizes)
            / sizeof(payload_sizes[0]);
        ++i
    ) {
        int rc = run_case(
            ctx,
            payload_sizes[i]
        );

        if (rc != 0) {
            mb_destroy(ctx);

            fprintf(
                stderr,
                "benchmark case failed: %d\n",
                rc
            );

            return rc;
        }
    }

    mb_destroy(ctx);

    if (bench_sink == UINT64_MAX) {
        printf(
            "sink=%llu\n",
            (unsigned long long)bench_sink
        );
    }

    return 0;
}