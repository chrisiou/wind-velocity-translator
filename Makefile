.POSIX:
.SUFFIXES:

SRC_DIR=include
CXX=g++
CXXFLAGS=-Wall -Wextra
CXXSTD=--std=c++11

all: beaufort.x

beaufort.x: translator.cpp $(SRC_DIR)/velocities_to_beaufort.cpp
	$(CXX) -o $@ $^ 

run:
	./beaufort.x $(arg)

clean:
	rm beaufort.x

.PHONY: all clean