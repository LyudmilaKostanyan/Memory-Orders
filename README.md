# Thread-Safe Counter and Memory Order Exploration

## Overview
This project explores the behavior of shared counters in multithreaded C++ applications, demonstrating how different synchronization strategies affect correctness and performance. It compares:

- Atomic operations with different `std::memory_order` settings
- Mutex-based synchronization
- A single-threaded baseline
- A non-thread-safe version (no synchronization)

The main goal is to understand memory consistency models and thread safety in practice using `std::atomic<int>` and standard C++11 concurrency features.

## Problem Description
We want to increment a shared counter concurrently from multiple threads. The challenge is to do this **safely and efficiently**, avoiding data races. The program:

- Launches a number of threads equal to the hardware concurrency level.
- Each thread increments a shared counter a fixed number of times (e.g., 5,000,000).
- Tests several approaches:
  - **No synchronization** (raw `int`, to demonstrate data races)
  - **Mutex protection**
  - **`std::atomic` with various memory orders**
  - **Single-threaded baseline**
- Measures execution time and final counter value for each approach.

This setup lets you observe how memory order affects correctness and performance.

## Memory Orders Explained
In C++, `std::atomic` allows specifying memory ordering semantics that affect how operations on atomic variables are seen by different threads. Here’s a brief summary:

| Memory Order       | Description                                                                 |
|--------------------|-----------------------------------------------------------------------------|
| `memory_order_relaxed`     | Only guarantees atomicity. No ordering or visibility guarantees.       |
| `memory_order_acquire`     | Prevents reordering of subsequent reads/writes before the load.        |
| `memory_order_release`     | Prevents reordering of prior reads/writes after the store.             |
| `memory_order_acq_rel`     | Combines `acquire` on load and `release` on store.                     |
| `memory_order_seq_cst`     | Provides a single total order visible to all threads. Strictest model. |

Understanding these orders is critical when writing performant and correct lock-free concurrent code.

## Example Output (8 Threads, 5,000,000 Iterations Each)
```
Threads: 8
Iteration: 5000000

Memory Order             Multithreaded (ms)       Counter Values
---------------------------------------------------------------------------
NonAtomic                95                       8083377
SingleThreaded           110                      40000000
Relaxed                  658                      40000000
Acquire                  708                      40000000
Release                  742                      40000000
AcquireRelease           806                      40000000
Sequential               818                      40000000
WithMutex                414975                   40000000
```

### Output Breakdown
- **NonAtomic**: Demonstrates a data race. Counter is incorrect (`8083377 << 40000000`).
- **SingleThreaded**: Correct. No overhead from threading. Acts as a baseline.
- **Relaxed**: Fastest atomic method but only guarantees atomicity. Still correct due to simplicity of operation.
- **Acquire/Release/AcqRel**: More ordering guarantees → slightly slower.
- **Sequential**: Ensures global visibility order. Safest but slower.
- **WithMutex**: Correct but very slow due to locking overhead on every increment.

## Explanation
### Why `NonAtomic` is wrong:
- Multiple threads modify the same variable without coordination → lost updates.

### Why `Relaxed` is correct:
- Each thread only uses `fetch_add`, which is atomic. No intermediate shared state → atomicity is enough.

### Why stricter orders are slower:
- Stronger memory guarantees introduce fences and synchronization that slow down execution.

### Why Mutex is slowest:
- Locking every increment creates **massive contention**.

## How to Compile and Run
Assuming you're using CMake:

### Clone the Repository
```bash
git clone https://github.com/username/Memory-Orders.git
cd Memory-Orders
```

### Build
```bash
cmake -S . -B build
cmake --build build
```

### Run
```bash
cd build
./main
```
