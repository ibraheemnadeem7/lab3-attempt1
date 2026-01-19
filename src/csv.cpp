#include "csv.h"

#include <fstream>
#include <stdexcept>
#include <unordered_map>

static std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    bool in_quotes = false;

    for (std::size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (in_quotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    cur.push_back('"');
                    i++;
                } else {
                    in_quotes = false;
                }
            } else {
                cur.push_back(c);
            }
        } else {
            if (c == ',') {
                fields.push_back(cur);
                cur.clear();
            } else if (c == '"') {
                in_quotes = true;
            } else {
                cur.push_back(c);
            }
        }
    }
    fields.push_back(cur);
    return fields;
}

static double to_double(const std::string& s, const std::string& what) {
    try {
        size_t idx = 0;
        double v = std::stod(s, &idx);
        (void)idx;
        return v;
    } catch (...) {
        throw std::runtime_error("Failed to parse " + what + " as double: '" + s + "'");
    }
}

std::vector<DemandPoint> load_demand_points_csv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Could not open input CSV: " + path);

    std::string header;
    if (!std::getline(in, header)) throw std::runtime_error("CSV appears empty: " + path);

    const auto h = split_csv_line(header);
    std::unordered_map<std::string, std::size_t> col;
    for (std::size_t i = 0; i < h.size(); i++) col[h[i]] = i;

    auto require_col = [&](const std::string& name) -> std::size_t {
        auto it = col.find(name);
        if (it == col.end()) {
            throw std::runtime_error("Missing column '" + name + "' in CSV header. Header was: " + header);
        }
        return it->second;
    };

    // Expect the cleaned format produced by osm_wifi_prep:
    // id,name,lat,lon,x_m,y_m
    const std::size_t c_id  = require_col("id");
    const std::size_t c_nm  = require_col("name");
    const std::size_t c_lat = require_col("lat");
    const std::size_t c_lon = require_col("lon");
    const std::size_t c_x   = require_col("x_m");
    const std::size_t c_y   = require_col("y_m");

    std::vector<DemandPoint> pts;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        const auto f = split_csv_line(line);
        if (f.size() < h.size()) continue;

        DemandPoint p;
        p.id = f[c_id];
        p.name = f[c_nm];
        p.lat = to_double(f[c_lat], "lat");
        p.lon = to_double(f[c_lon], "lon");
        p.x_m = to_double(f[c_x], "x_m");
        p.y_m = to_double(f[c_y], "y_m");
        pts.push_back(std::move(p));
    }

    if (pts.empty()) throw std::runtime_error("No data rows found in CSV: " + path);
    return pts;
}
