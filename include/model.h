#pragma once

#include <string>

struct DemandPoint {
    std::string id;
    std::string name;
    double lat = 0.0;
    double lon = 0.0;
    double x_m = 0.0;
    double y_m = 0.0;
};

struct AccessPoint {
    std::string label;
    double x_m = 0.0;
    double y_m = 0.0;
};
