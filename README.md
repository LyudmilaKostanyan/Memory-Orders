# Thread-Safe Counter with Atomics

## Overview
This project implements a thread-safe counter using `std::atomic<int>` with `memory_order_seq_cst` in C++. It spawns multiple threads to increment a shared counter in parallel, then compares the results and performance against a single-threaded implementation. The goal is to demonstrate the correctness and efficiency of atomic operations in a concurrent environment, showcasing how concurrency relies on well-defined memory operations.

## Problem Description
The task is to create a shared counter that multiple threads can increment safely without data races. Using `std::atomic<int>` with sequential consistency (`memory_order_seq_cst`), the program:
- Spawns a number of threads equal to the hardware's concurrency limit (e.g., 8 on an 8-core system).
- Each thread increments the counter 100,000,000 times.
- Measures the final count and execution time for the multithreaded version.
- Compares this to a single-threaded version incrementing the counter the same total number of times (e.g., 800,000,000 for 8 threads).
- Outputs a table showing the counter values and performance metrics (execution time, difference, and speedup).

This exercise highlights the importance of atomic operations for thread safety and evaluates the performance trade-offs of parallelism.

## Example Output
Below is an example output from running the program on an 8-core system:

```
Metric                   Multithreaded       Single-threaded     Difference          Speed-Up       
----------------------------------------------------------------------------------------------------
Counter Values           800000000           800000000           0                                  
Execution Time (ms)      17349               2212                15137               7.84313        
```

## Explanation of Output
- **Counter Values**:
  - `Multithreaded`: The final value of the `std::atomic<int> counter` after all threads complete their increments (e.g., 800,000,000). With 8 threads each incrementing 100,000,000 times, this matches the expected total.
  - `Single-threaded`: The final value of the regular `int count` after incrementing it `num_threads * increments_per_thread` times sequentially (e.g., 800,000,000).
  - `Difference`: The absolute difference between the multithreaded and single-threaded values (e.g., 0). A value of 0 confirms the atomic counter is thread-safe and correct, as both methods achieve the same result.
  - `Speed-Up`: Left blank in this row (as shown in your output with a space), since speedup doesn’t apply to counter values—it’s a correctness metric, not a performance one.

- **Execution Time (ms)**:
  - `Multithreaded`: Time taken for all threads to complete their increments (e.g., 17,349 ms). This includes thread creation, synchronization overhead from `fetch_add`, and parallel execution across cores.
  - `Single-threaded`: Time taken to increment a regular integer sequentially for the same total number of increments (e.g., 2,212 ms). This runs on a single core without threading overhead.
  - `Difference`: Multithreaded time minus single-threaded time (e.g., 17,349 - 2,212 = 15,137 ms). A positive difference indicates the multithreaded version took longer, which is unexpected for parallelism and suggests significant overhead (see below for discussion).
  - `Speed-Up`: Calculated in your code as `multithreaded / singlethreaded` (e.g., 17,349 / 2,212 ≈ 7.84313). This is a misnomer:
    - **Correction**: In performance analysis, "speedup" typically means `single-threaded time / multithreaded time`, where a value > 1 indicates the parallel version is faster. Here, it should be 2,212 / 17,349 ≈ 0.1275, meaning the multithreaded version is ~7.8 times *slower*. The current output (7.84313) reflects the inverse, which could confuse readers expecting a traditional speedup metric.

### Why the Multithreaded Version is Slower
The example output shows the multithreaded execution (17,349 ms) taking much longer than the single-threaded one (2,212 ms), which is counterintuitive for parallelism. This likely stems from:
1. **Contention on the Atomic Counter**: With 8 threads repeatedly calling `fetch_add` on the same `std::atomic<int>` with `memory_order_seq_cst`, there’s heavy contention. Sequential consistency enforces a total order across all threads, causing frequent cache invalidation and synchronization delays.
2. **Overhead**: Thread creation, context switching, and atomic operation costs outweigh the benefits of parallelism for this workload.
3. **Workload Nature**: Incrementing a counter is a trivial operation with high contention and no independent computation, making it a poor candidate for parallelism compared to tasks with more substantial per-thread work.

## How to Compile and Run the Code
This section assumes the code is part of a repository and uses CMake for building. Adjust the repository URL and directory names as needed.

### Clone the Repository
If you haven't cloned the repository yet, do so by running:

```bash
git clone https://github.com/username/Thread-Safe-Counter-Experiment.git
cd Thread-Safe-Counter-Experiment
```

Replace `username` with the actual GitHub username hosting the repository.

### Build the Project
Once in the project directory, compile the code with CMake:

```bash
cmake -S . -B build
cmake --build build
```

This generates the executable (e.g., `main`) in the `build` directory. Ensure you have the following installed:
- **CMake**: For building the project.
- **C++ Compiler**: E.g., `g++` (supporting C++11 or later).
- **Standard C++ Library**: Included with most compilers.

### Run the Compiled Executable
After building, run the program:

```bash
cd build
./main  # On Windows, use `main.exe`
```
