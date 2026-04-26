#include <vector>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>

using sum_t = int64_t;

// forward declarations (defined in sum.cpp)
sum_t sum_baseline(const std::vector<int>& a);
sum_t sum_divide_conquer_seq(const std::vector<int>& a);
sum_t sum_divide_conquer_forkjoin(const std::vector<int>& a);
sum_t sum_divide_conquer_simd(const std::vector<int>& a);

using clock_t2 = std::chrono::high_resolution_clock;

static double ms(clock_t2::time_point t0, clock_t2::time_point t1) {
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

int main(int argc, char* argv[]) {
    size_t n    = (argc > 1) ? (size_t)std::atoll(argv[1]) : 10000000ULL;
    int    seed = (argc > 2) ? std::atoi(argv[2]) : 0;

    std::srand((unsigned)seed);
    std::vector<int> a(n);
    for (size_t i = 0; i < n; ++i)
        a[i] = std::rand() % 201 - 100; // values in [-100, 100]

    std::cout << "n = " << n << "  seed = " << seed << "\n\n";

    auto run = [&](const std::string& name, sum_t (*fn)(const std::vector<int>&)) -> double {
        auto t0  = clock_t2::now();
        sum_t s  = fn(a);
        auto t1  = clock_t2::now();
        double t = ms(t0, t1);
        std::cout << name << ": sum=" << s << "  time=" << t << " ms\n";
        return t;
    };

    double t_base = run("baseline     ", sum_baseline);
    double t_seq  = run("seq d&c      ", sum_divide_conquer_seq);
    double t_par  = run("forkjoin d&c ", sum_divide_conquer_forkjoin);
    double t_simd = run("simd d&c     ", sum_divide_conquer_simd);

    std::cout << "\nSpeedups vs baseline:\n";
    std::cout << "  seq:      " << t_base / t_seq  << "x\n";
    std::cout << "  forkjoin: " << t_base / t_par  << "x\n";
    std::cout << "  simd:     " << t_base / t_simd << "x\n";

    return 0;
}
