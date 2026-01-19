CXX ?= g++
CXXFLAGS ?= -std=c++17 -O3 -Wall -Wextra -pedantic

TARGET = wifi_planner_starter
SRC = src/main.cpp src/csv.cpp src/solvers.cpp
INC = -Iinclude

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) kenyonout.csv --radius 60 --solver one_at_first

clean:
	rm -f $(TARGET)

.PHONY: all clean run
