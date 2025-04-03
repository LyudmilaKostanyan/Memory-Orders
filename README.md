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
  - `Multithreaded`: The final value of the atomic counter after all threads complete (e.g., 800,000,000).
  - `Single-threaded`: The final value of a regular integer counter incremented sequentially (e.g., 800,000,000).
  - `Difference`: The absolute difference between the two (0 if correct, indicating thread safety).
  - No speedup is calculated for this row, as it’s a correctness check.

- **Execution Time (ms)**:
  - `Multithreaded`: Time taken for all threads to complete their increments (e.g., 17,349 ms).
  - `Single-threaded`: Time taken for the sequential increment (e.g., 2,212 ms).
  - `Difference`: Multithreaded time minus single-threaded time (e.g., 15,137 ms). A positive value indicates the multithreaded version took longer, likely due to thread overhead or contention.
  - `Speed-Up`: Calculated as `multithreaded / singlethreaded` (e.g., 7.84313). Note: This is unconventional; typically, speedup is `singlethreaded / multithreaded` (e.g., 0.1275 here), where >1 indicates a faster parallel execution. The current output suggests a performance penalty, which could occur due to atomic operation overhead or contention on the counter.

The results depend on hardware (e.g., number of cores) and system load. On highly contended systems, multithreaded performance may degrade due to synchronization costs.

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
