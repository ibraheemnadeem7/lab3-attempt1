CXX ?= g++
CXXFLAGS ?= -std=c++17 -O3 -Wall -Wextra -pedantic -fopenmp-simd
LDFLAGS   = -pthread

# Wi-Fi planner
WIFI_TARGET = wifi_planner_starter
WIFI_SRC    = src/main.cpp src/csv.cpp src/solvers.cpp
INC         = -Iinclude

# Vector-sum demo
VECSUM_TARGET = vecsum_demo
VECSUM_SRC    = src/vecsum_demo.cpp src/sum.cpp

all: $(WIFI_TARGET) $(VECSUM_TARGET)

$(WIFI_TARGET): $(WIFI_SRC)
	$(CXX) $(CXXFLAGS) $(INC) $(WIFI_SRC) -o $(WIFI_TARGET) $(LDFLAGS)

$(VECSUM_TARGET): $(VECSUM_SRC)
	$(CXX) $(CXXFLAGS) $(VECSUM_SRC) -o $(VECSUM_TARGET) $(LDFLAGS)

run: $(WIFI_TARGET)
	./$(WIFI_TARGET) kenyonout.csv --radius 60 --solver one_at_first

clean:
	rm -f $(WIFI_TARGET) $(VECSUM_TARGET)

.PHONY: all clean run
