#pragma once

#include "model.h"

#include <string>
#include <vector>

// Reads the cleaned CSV produced by osm_wifi_prep:
// id,name,lat,lon,x_m,y_m
std::vector<DemandPoint> load_demand_points_csv(const std::string& path);
