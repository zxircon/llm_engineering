#include <iostream>
#include <vector>
#include <ctime>
#include <limits>
#include <iomanip>

class LCG {
    uint32_t value;
    static constexpr uint32_t a = 1664525;
    static constexpr uint32_t c = 1013904223;
    static constexpr uint32_t m = 1U << 32; // 2^32
public:
    LCG(uint32_t seed) : value(seed) {}

    uint32_t next() {
        value = (a * value + c) % m;
        return value;
    }
};

int max_subarray_sum(int n, uint32_t seed, int min_val, int max_val) {
    LCG lcg(seed);
    std::vector<int> random_numbers(n);
    for (int i = 0; i < n; ++i) {
        random_numbers[i] = lcg.next() % (max_val - min_val + 1) + min_val;
    }

    int max_sum = std::numeric_limits<int>::min();
    for (int i = 0; i < n; ++i) {
        int current_sum = 0;
        for (int j = i; j < n; ++j) {
            current_sum += random_numbers[j];
            if (current_sum > max_sum) {
                max_sum = current_sum;
            }
        }
    }
    return max_sum;
}

int total_max_subarray_sum(int n, uint32_t initial_seed, int min_val, int max_val) {
    int total_sum = 0;
    LCG lcg(initial_seed);
    for (int i = 0; i < 20; ++i) {
        uint32_t seed = lcg.next();
        total_sum += max_subarray_sum(n, seed, min_val, max_val);
    }
    return total_sum;
}

int main() {
    int n = 10000;         // Number of random numbers
    uint32_t initial_seed = 42; // Initial seed for the LCG
    int min_val = -10;     // Minimum value of random numbers
    int max_val = 10;      // Maximum value of random numbers

    std::clock_t start_time = std::clock();
    int result = total_max_subarray_sum(n, initial_seed, min_val, max_val);
    std::clock_t end_time = std::clock();

    std::cout << "Total Maximum Subarray Sum (20 runs): " << result << std::endl;
    std::cout << "Execution Time: " << std::fixed << std::setprecision(6)
              << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << std::endl;
    
    return 0;
}
