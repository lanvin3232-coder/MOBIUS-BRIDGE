# MÖBIUS Black-Box Benchmark Evidence v0.2

## Status

**PASS**

A restricted private capability was invoked through the public
MOBIUS-BRIDGE ABI and measured using the MMDC Black-Box Benchmark v0.2.

The private implementation is not included in this repository.

---

## Execution Path

The measured execution path was:

```text
Black-Box Benchmark
        │
        ▼
MOBIUS-BRIDGE ABI
        │
        ▼
mb_run()
        │
        ▼
==============================
       PRIVATE BOUNDARY
==============================
        │
        ▼
Private Adapter
        │
        ▼
MMDC v0.1 Restricted Capability
        │
        ▼
Minimal Result
```

The benchmark does not call the private capability directly.

All timed capability invocations pass through the MOBIUS-BRIDGE interface.

---

## Public ABI

```text
ABI version : 0.1.0
Exports     : 3
```

Exported symbols:

```text
mb_create
mb_run
mb_destroy
```

---

## Benchmark Identity

```text
Protocol   : MOBIUS-BLACKBOX-0.2
Benchmark  : MMDC-BENCH-V0.2
Capability : MMDC-v0.1
Status     : PASS
```

---

## Functional Verification

```text
Checks                : 3
Successful            : 3
Failures               : 0

Deterministic          : PASS
Hard gate              : PASS
All-invalid rejection  : PASS
```

The functional checks verify that:

```text
hard-invalid candidates remain rejected

all-invalid candidate sets do not return
a "best bad" candidate

repeated identical requests produce
identical functional results
```

---

## Measurement Configuration

```text
Warmup calls    : 10,000
Batch size      : 1,000 bridge calls
Batch count     : 1,000
Timed calls     : 1,000,000
Timer samples   : 10,000
```

One timer pair surrounds each batch of 1,000 bridge calls.

This reduces timer measurement overhead per individual bridge invocation.

---

## Timer Baseline

Observed timer-pair baseline:

```text
Mean : 32 ns
p50  : 30 ns
p95  : 40 ns
p99  : 40 ns
Min  : 20 ns
Max  : 12,970 ns
```

Amortized across a 1,000-call benchmark batch:

```text
Timer overhead per call:
0.032 ns
```

---

## Bridge Performance Result

Observed across:

```text
1,000,000 timed MOBIUS-BRIDGE invocations
```

Results:

```text
Raw mean / call           : 34 ns
Timer-amortized / call    : 0.032 ns
Reported net mean / call  : 33.968 ns

Batch-average p50 / call  : 33 ns
Batch-average p95 / call  : 41 ns
Batch-average p99 / call  : 53 ns

Batch-average min / call  : 32 ns
Batch-average max / call  : 93 ns

Observed throughput       : 28,998,382.528 ops/s
```

The throughput-derived overall mean is approximately:

```text
34.48 ns / bridge invocation
```

The difference from the integer `raw_mean_ns_per_call` value is caused by
integer truncation in the benchmark summary.

---

## Percentile Interpretation

The p50, p95, and p99 values above are **batch-average per-call values**.

Each measurement sample represents:

```text
1,000 bridge calls
        │
        ▼
one elapsed batch time
        │
        ▼
elapsed time / 1,000
        │
        ▼
batch-average ns / call
```

Therefore:

```text
p99 = 53 ns
```

must not be interpreted as the p99 latency of one million individually timed
calls.

It is the p99 of the 1,000 batch-average-per-call samples.

---

## Measurement Boundary

The measured path includes:

```text
timer start
    │
    ▼
mb_run()
    │
    ▼
private adapter
    │
    ▼
restricted private MMDC capability
    │
    ▼
minimal result
    │
    ▼
timer stop
```

The result does not represent Internet or HTTP latency.

The following are outside the benchmark boundary:

```text
Internet RTT
DNS
TLS handshake
HTTP processing
external request queues
client rendering
```

---

## Result Interpretation

This benchmark demonstrates that the restricted MMDC v0.1 capability:

```text
executed behind the MOBIUS-BRIDGE ABI
passed the defined functional invariants
processed 1,000,000 timed bridge invocations
produced deterministic benchmark outputs
rejected hard-invalid cases
rejected all-invalid candidate sets
```

and that the observed benchmark run produced approximately:

```text
34.5 ns mean bridge invocation time
~29.0 million bridge invocations / second
```

under the recorded benchmark environment.

---

## Scope

This evidence applies only to:

```text
MMDC v0.1 restricted private capability
MOBIUS-BRIDGE ABI 0.1.0
MMDC-BENCH-V0.2
the specific benchmark environment
```

It does **not** claim that:

```text
the complete private engine runs at 34 ns

the complete research system has been benchmarked

the public bridge contains the private algorithm

network requests complete in 34 ns

all workloads have identical latency

the benchmark represents arbitrary real-world workloads
```

---

## Private Boundary

The following remain private:

```text
implementation source
private decision logic
internal thresholds
internal coefficients
private state
private architecture
optimization details
private repository contents
```

Only the standardized bridge boundary and sanitized benchmark evidence are
published.

---

## Binary Identity

The benchmark result should be bound to the SHA-256 digest of the exact
private bridge binary used by the successful v0.2 run.

```text
Private bridge SHA-256:
<INSERT_CURRENT_V0_2_PRIVATE_BRIDGE_SHA256>
```

The digest identifies the binary.

It does not reveal the implementation.

---

## Verification Summary

```text
Protocol          : MOBIUS-BLACKBOX-0.2
Benchmark         : MMDC-BENCH-V0.2
Capability        : MMDC-v0.1
ABI               : 0.1.0

Functional        : PASS
Determinism       : PASS
Hard gate         : PASS
All-invalid       : PASS

Timed calls       : 1,000,000
Mean observed     : ~34.48 ns / call
Batch p50         : 33 ns / call
Batch p95         : 41 ns / call
Batch p99         : 53 ns / call
Throughput        : ~29.0 M ops/s

Overall status    : PASS
```

---

## Principle

> Measure through the bridge. Publish the evidence. Keep the mechanism private.