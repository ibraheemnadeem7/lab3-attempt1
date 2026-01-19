#pragma once

#include "model.h"

#include <cstddef>
#include <string>
#include <vector>

struct CoverageStats {
    std::size_t total_points = 0;
    std::size_t covered_points = 0;
    double coverage_percent = 0.0;
};

struct Solution {
    std::string solver_name;
    double radius_m = 0.0;
    std::vector<AccessPoint> aps;
    CoverageStats stats;
};
