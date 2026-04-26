#include <vector>
#include <future>
#include <cstddef>
#include <cstdint>

using sum_t = int64_t;

static const size_t CUTOFF     = 4096;
static const size_t PAR_CUTOFF = 100000;

// ── baseline ────────────────────────────────────────────────────────────────

sum_t sum_baseline(const std::vector<int>& a) {
    sum_t s = 0;
    for (size_t i = 0; i < a.size(); ++i)
        s += (sum_t)a[i];
    return s;
}

// ── sequential divide-and-conquer ───────────────────────────────────────────

static sum_t seq_helper(const std::vector<int>& a, size_t lo, size_t hi) {
    if (hi - lo <= CUTOFF) {
        sum_t s = 0;
        for (size_t i = lo; i < hi; ++i)
            s += (sum_t)a[i];
        return s;
    }
    size_t mid  = lo + (hi - lo) / 2;
    sum_t left  = seq_helper(a, lo, mid);
    sum_t right = seq_helper(a, mid, hi);
    return left + right;
}

sum_t sum_divide_conquer_seq(const std::vector<int>& a) {
    return seq_helper(a, 0, a.size());
}

// ── fork-join parallel divide-and-conquer ───────────────────────────────────

static sum_t forkjoin_helper(const std::vector<int>& a, size_t lo, size_t hi) {
    if (hi - lo <= PAR_CUTOFF) {
        sum_t s = 0;
        for (size_t i = lo; i < hi; ++i)
            s += (sum_t)a[i];
        return s;
    }
    size_t mid = lo + (hi - lo) / 2;

    auto left_future = std::async(
        std::launch::async,
        forkjoin_helper,
        std::cref(a), lo, mid
    );

    sum_t right = forkjoin_helper(a, mid, hi);
    sum_t left  = left_future.get();
    return left + right;
}

sum_t sum_divide_conquer_forkjoin(const std::vector<int>& a) {
    return forkjoin_helper(a, 0, a.size());
}

// ── SIMD divide-and-conquer ─────────────────────────────────────────────────

static sum_t simd_helper(const std::vector<int>& a, size_t lo, size_t hi) {
    if (hi - lo <= CUTOFF) {
        sum_t s = 0;
#pragma omp simd reduction(+:s)
        for (size_t i = lo; i < hi; ++i)
            s += (sum_t)a[i];
        return s;
    }
    size_t mid  = lo + (hi - lo) / 2;
    sum_t left  = simd_helper(a, lo, mid);
    sum_t right = simd_helper(a, mid, hi);
    return left + right;
}

sum_t sum_divide_conquer_simd(const std::vector<int>& a) {
    return simd_helper(a, 0, a.size());
}
