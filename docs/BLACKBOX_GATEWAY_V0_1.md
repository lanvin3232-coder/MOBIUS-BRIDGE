# MÖBIUS Black-Box Test Gateway v0.1

## Purpose

The MÖBIUS Black-Box Test Gateway provides a public verification surface for a
restricted private capability operating behind the MOBIUS-BRIDGE ABI.

The objective is simple:

> Allow an external tester to request a controlled benchmark and receive
> measurable results without receiving the proprietary implementation.

The private engine source, internal decision trace, coefficients, thresholds,
and architecture remain private.

---

## Architecture

```text
External Tester
      │
      ▼
Public Test Request
      │
      ▼
Black-Box Gateway
      │
      ▼
MOBIUS-BRIDGE ABI
      │
      ▼
==============================
       PRIVATE BOUNDARY
==============================
      │
      ▼
Restricted Private Engine
      │
      ▼
Measured Final Result
      │
      ▼
Verification Receipt
```

---

## Security Rule

The gateway exposes results, not mechanisms.

The following must never be returned:

```text
private source code
private weights
private thresholds
private score vectors
intermediate decision state
reasoning traces
memory contents
private repository identifiers
private file paths
private symbols
internal architecture topology
```

---

## Public Test Modes

v0.1 defines three controlled test modes.

### 1. Functional

Verifies that the private engine can process a standardized test suite through
the public bridge boundary.

```text
mode = functional
```

Returned measurements may include:

- total cases
- successful cases
- rejected cases
- deterministic repeatability
- output consistency
- final PASS / FAIL

---

### 2. Performance

Measures server-side execution of the restricted private capability.

```text
mode = performance
```

Returned measurements may include:

- warmup count
- measured case count
- total execution time
- mean execution time
- p50 latency
- p95 latency
- p99 latency
- minimum latency
- maximum latency
- operations per second

Network latency is not included in engine latency.

---

### 3. Combined

Runs both functional and performance verification.

```text
mode = combined
```

---

## Standard Benchmark

The first standard benchmark is:

```text
Benchmark ID : MMDC-BENCH-V0.1
Warmup       : 10,000
Cases        : 100,000
Capability   : MMDC v0.1
ABI          : MOBIUS-BRIDGE 0.1.0
```

The benchmark dataset is generated from a fixed versioned procedure.

A benchmark result is comparable only when the same benchmark ID and version
are used.

---

## Request

A public request uses the following logical structure:

```json
{
  "protocol": "MOBIUS-BLACKBOX-0.1",
  "benchmark": "MMDC-BENCH-V0.1",
  "mode": "combined"
}
```

v0.1 intentionally does not permit arbitrary access to private engine state.

---

## Response

A successful response uses the following logical structure:

```json
{
  "protocol": "MOBIUS-BLACKBOX-0.1",
  "benchmark": "MMDC-BENCH-V0.1",
  "abi": "0.1.0",
  "capability": "MMDC-v0.1",

  "status": "PASS",

  "functional": {
    "cases": 100000,
    "successful": 100000,
    "failures": 0,
    "deterministic": true,
    "hard_gate": true,
    "all_invalid_rejection": true
  },

  "performance": {
    "warmup": 10000,
    "cases": 100000,

    "mean_ns": 0,
    "p50_ns": 0,
    "p95_ns": 0,
    "p99_ns": 0,
    "min_ns": 0,
    "max_ns": 0,

    "operations_per_second": 0
  },

  "binary": {
    "sha256": "<private-binary-digest>"
  },

  "receipt": {
    "id": "<verification-receipt-id>",
    "timestamp": "<UTC timestamp>"
  }
}
```

The zero values above are schema placeholders.

They are not benchmark claims.

---

## Measurement Boundary

Engine execution latency is measured around the private capability invocation.

```text
timer start
    │
    ▼
MOBIUS-BRIDGE
    │
    ▼
private capability
    │
    ▼
result produced
    │
    ▼
timer stop
```

The following are excluded from engine latency:

```text
Internet RTT
TLS handshake
HTTP parsing
DNS resolution
client rendering
external queue delay
```

If end-to-end network latency is reported, it must be labeled separately.

---

## Determinism

For deterministic benchmark cases:

```text
same benchmark version
        +
same case
        +
same binary
        ↓
same functional result
```

The gateway may repeat selected cases multiple times to verify deterministic
output.

---

## Hard-Gate Verification

MMDC-BENCH-V0.1 includes cases designed to verify that:

```text
hard-invalid candidate
        ↓
rejected
        ↓
cannot recover through lower-priority metrics
```

The suite also includes all-invalid cases.

For an all-invalid case, returning a "best bad" candidate is considered a
functional failure.

---

## Binary Identity

Each benchmark response identifies the private binary by SHA-256.

Example:

```text
SHA-256:
d039374f1cb0f0b8320256887eb6b0d6e770c18faee0cd2d17dbe92f07c82d31
```

The digest identifies the exact binary associated with the reported result.

A different binary is expected to produce a different digest.

---

## Verification Receipt

A benchmark result should eventually be accompanied by a signed verification
receipt.

The receipt should bind together:

```text
protocol version
benchmark ID
binary SHA-256
result status
measurement summary
timestamp
receipt ID
```

This allows a third party to verify that the published result belongs to a
specific benchmark run and private binary.

---

## Comparison Rules

Performance comparisons are valid only when the following are recorded:

```text
benchmark version
runner / CPU class
operating system
compiler
build mode
case count
warmup count
binary digest
```

Results from different environments must not be presented as directly
equivalent without qualification.

---

## What This Gateway Can Demonstrate

The gateway can provide evidence that:

- a restricted private capability actually executed
- the public bridge successfully invoked it
- standardized functional cases passed or failed
- deterministic behavior was observed
- hard-invalid cases were rejected
- measured latency and throughput were observed
- a specific private binary produced the result

---

## What This Gateway Does Not Reveal

The gateway does not reveal:

- how the private algorithm works internally
- the complete private research system
- private scoring functions
- private architecture details
- proprietary optimization methods

---

## Current Private Binary

```text
Capability : MMDC v0.1
ABI        : MOBIUS-BRIDGE 0.1.0
SHA-256    : d039374f1cb0f0b8320256887eb6b0d6e770c18faee0cd2d17dbe92f07c82d31
E2E        : PASS
```

---

## Principle

> Let outsiders measure the capability without giving them the mechanism.