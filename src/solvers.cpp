#include "solvers.h"

#include <algorithm>
#include <cmath>
#include <random>

static double dist2(double ax, double ay, double bx, double by) {
    const double dx = ax - bx;
    const double dy = ay - by;
    return dx*dx + dy*dy;
}

CoverageStats compute_coverage(const std::vector<DemandPoint>& pts, const std::vector<AccessPoint>& aps, double radius_m) {
    CoverageStats st;
    st.total_points = pts.size();
    if (pts.empty()) return st;

    const double r2 = radius_m * radius_m;
    std::size_t covered = 0;
    for (const auto& p : pts) {
        bool ok = false;
        for (const auto& ap : aps) {
            if (dist2(p.x_m, p.y_m, ap.x_m, ap.y_m) <= r2) {
                ok = true;
                break;
            }
        }
        if (ok) covered++;
    }
    st.covered_points = covered;
    st.coverage_percent = 100.0 * static_cast<double>(covered) / static_cast<double>(pts.size());
    return st;
}

Solution solve_none(const std::vector<DemandPoint>& pts, double radius_m) {
    Solution s;
    s.solver_name = "none";
    s.radius_m = radius_m;
    s.aps.clear();
    s.stats = compute_coverage(pts, s.aps, radius_m);
    return s;
}

Solution solve_one_at_first(const std::vector<DemandPoint>& pts, double radius_m) {
    Solution s;
    s.solver_name = "one_at_first";
    s.radius_m = radius_m;

    if (!pts.empty()) {
        AccessPoint ap;
        ap.label = pts[0].name.empty() ? pts[0].id : pts[0].name;
        ap.x_m = pts[0].x_m;
        ap.y_m = pts[0].y_m;
        s.aps.push_back(ap);
    }

    s.stats = compute_coverage(pts, s.aps, radius_m);
    return s;
}

Solution solve_random_k(const std::vector<DemandPoint>& pts, double radius_m, std::size_t k, std::uint64_t seed) {
    Solution s;
    s.solver_name = "random_k";
    s.radius_m = radius_m;

    if (pts.empty() || k == 0) {
        s.stats = compute_coverage(pts, s.aps, radius_m);
        return s;
    }

    std::mt19937_64 rng(seed);
    std::vector<std::size_t> idx(pts.size());
    for (std::size_t i = 0; i < pts.size(); i++) idx[i] = i;
    std::shuffle(idx.begin(), idx.end(), rng);

    if (k > idx.size()) k = idx.size();
    s.aps.reserve(k);
    for (std::size_t i = 0; i < k; i++) {
        const auto& p = pts[idx[i]];
        AccessPoint ap;
        ap.label = p.name.empty() ? p.id : p.name;
        ap.x_m = p.x_m;
        ap.y_m = p.y_m;
        s.aps.push_back(ap);
    }

    s.stats = compute_coverage(pts, s.aps, radius_m);
    return s;
}

std::vector<std::string> available_solvers() {
    return {"none", "one_at_first", "random_k"};
}
