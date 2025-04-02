#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <iomanip>
#include <cmath>
#include "kaizen.h"

auto num_threads = std::thread::hardware_concurrency();
constexpr int increments_per_thread = 100000000;

std::atomic<int> counter(0);

void increment_counter() {
    for (int i = 0; i < increments_per_thread; ++i)
        counter.fetch_add(1, std::memory_order_seq_cst);
}

void print_table_header() {
    std::cout << std::setw(25) << std::left << "Metric"
              << std::setw(20) << "Multithreaded"
              << std::setw(20) << "Single-threaded"
              << std::setw(20) << "Difference"
              << std::setw(15) << "Speed-Up"
              << "\n";
    std::cout << std::string(100, '-') << "\n";
}

void print_table_row(const std::string& metric, double multithreaded, double singlethreaded) {
    double difference = multithreaded - singlethreaded;
    double speedup = singlethreaded ? multithreaded / singlethreaded : 0;
    
    std::cout << std::setw(25) << std::left << metric
              << std::setw(20) << multithreaded
              << std::setw(20) << singlethreaded
              << std::setw(20) << difference
              << std::setw(15) << speedup
              << "\n";
}

void print_value_row(int multithreaded, int singlethreaded) {
    int difference = multithreaded - singlethreaded;
    
    std::cout << std::setw(25) << std::left << "Counter Values"
              << std::setw(20) << multithreaded
              << std::setw(20) << singlethreaded
              << std::setw(20) << std::abs(difference)
              << std::setw(15) << " "
              << "\n";
}

int main() {
    std::vector<std::thread> threads;
    auto timer = zen::timer();
    
    timer.start();
    for (int i = 0; i < num_threads; ++i)
        threads.emplace_back(increment_counter);
    
    for (auto &t : threads)
        t.join();
    
    timer.stop();
    double duration_multithreaded = timer.duration<zen::timer::msec>().count();
    int final_multithreaded = counter.load(std::memory_order_seq_cst);
    
    counter.store(0, std::memory_order_seq_cst);
    
    int count = 0;
    timer.start();
    
    for (int i = 0; i < num_threads * increments_per_thread; ++i)
        count++;
    
    timer.stop();
    double duration_singlethreaded = timer.duration<zen::timer::msec>().count();
    int final_singlethreaded = count;
    
    print_table_header();
    print_value_row(final_multithreaded, final_singlethreaded);
    print_table_row("Execution Time (ms)", duration_multithreaded, duration_singlethreaded);
    
    return 0;
}

