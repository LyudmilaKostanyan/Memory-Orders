#include <atomic>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <mutex>
#include "kaizen.h"

std::atomic<size_t> shared_counter(0);
std::mutex mtx;

constexpr int ITERATIONS_PER_THREAD = 5'000'000;

void perform_operations(std::memory_order order) {
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i)
        shared_counter.fetch_add(1, order);
}

void perform_operations_with_mutex(int thread_count) {
    static int counter = 0;

    counter = 0;

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < ITERATIONS_PER_THREAD; ++j) {
                std::lock_guard<std::mutex> lock(mtx);
                ++counter;
            }
        });
    }

    for (auto& t : threads)
        t.join();

    std::lock_guard<std::mutex> lock(mtx);
    shared_counter = counter;
}

void perform_operations_single_threaded(int thread_count) {
    int counter = 0;

    for (int i = 0; i < thread_count * ITERATIONS_PER_THREAD; ++i)
        ++counter;

    shared_counter = counter;
}

void perform_operations_non_atomic(int thread_count) {
    static int counter = 0;
    counter = 0;

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < ITERATIONS_PER_THREAD; ++j)
                ++counter;
        });
    }

    for (auto& t : threads)
        t.join();

    shared_counter = counter;
}

void run_test(const char* label, std::memory_order order, int thread_count) {
    shared_counter = 0;

    auto timer = zen::timer();
    timer.start();

    std::vector<std::thread> workers;
    for (int i = 0; i < thread_count; ++i)
        workers.emplace_back(perform_operations, order);

    for (auto& thread : workers)
        thread.join();

    timer.stop();
    double duration = timer.duration<zen::timer::msec>().count();

    std::cout << std::setw(25) << std::left << label
              << std::setw(25) << duration
              << std::setw(25) << shared_counter.load()
              << std::endl;
}

void run_custom_test(const char* label, void(*func)(int), int thread_count) {
    shared_counter = 0;

    auto timer = zen::timer();
    timer.start();
    func(thread_count);
    timer.stop();
    double duration = timer.duration<zen::timer::msec>().count();

    std::cout << std::setw(25) << std::left << label
              << std::setw(25) << duration
              << std::setw(25) << shared_counter.load()
              << std::endl;
}

int main() {
    unsigned int thread_count = std::thread::hardware_concurrency();
    if (thread_count == 0)
        thread_count = 8;

    std::cout << "Threads: " << thread_count << std::endl;
    std::cout << "Iteration: " << ITERATIONS_PER_THREAD << std::endl << std::endl;
    std::cout << std::setw(25) << std::left << "Memory Order"
              << std::setw(25) << "Multithreaded (ms)"
              << std::setw(25) << "Counter Values"
              << std::endl;
    std::cout << std::string(75, '-') << "\n";

    run_custom_test("NonAtomic", perform_operations_non_atomic, thread_count);
    run_custom_test("SingleThreaded", perform_operations_single_threaded, thread_count);
    run_test("Relaxed", std::memory_order_relaxed, thread_count);
    run_test("Acquire", std::memory_order_acquire, thread_count);
    run_test("Release", std::memory_order_release, thread_count);
    run_test("AcquireRelease", std::memory_order_acq_rel, thread_count);
    run_test("Sequential", std::memory_order_seq_cst, thread_count);
    run_custom_test("WithMutex", perform_operations_with_mutex, thread_count);

    return 0;
}
