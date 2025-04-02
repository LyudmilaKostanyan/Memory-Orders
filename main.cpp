#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

auto num_threads = std::thread::hardware_concurrency();
constexpr int increments_per_thread = 1000000;

std::atomic<int> counter(0);

void increment_counter() {
    for (int i = 0; i < increments_per_thread; ++i) {
        counter.fetch_add(1, std::memory_order_seq_cst);
    }
}

int main() {
    std::vector<std::thread> threads;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment_counter);
    }
    
    for (auto &t : threads) {
        t.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    
    std::cout << "Number of threads: " << num_threads << "\n";
    std::cout << "Final counter value: " << counter.load(std::memory_order_seq_cst) << "\n";
    std::cout << "Execution time: " << duration.count() << " seconds\n";
    
    int count = 0;
    start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads * increments_per_thread; ++i)
        count++;
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    std::cout << "Count without threads: " << count << "\n";
    std::cout << "Execution time without threads: " << duration.count() << " seconds\n";
    
    
    return 0;
}
