CXX = g++
CXXFLAGS = -Wall -Wextra -O0 -g -std=c++20
#CXXFLAGS = -Wall -Wextra -O3 -std=c++20
LDFLAGS = -lgpiod

SRC_DIR = src
SRC = $(shell find $(SRC_DIR) -name '*.cpp')
OUT = stationfiftyone

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OUT)