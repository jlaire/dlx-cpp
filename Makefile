CXX ?= g++
CXXFLAGS = -Iinclude -std=c++11 -pedantic -Wall -O2

LIB_SRC = src/AlgorithmDLX.cpp src/LinkedMatrix.cpp

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/dlx $(LIB_SRC) src/main.cpp

examples:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/sudoku $(LIB_SRC) examples/Sudoku.cpp examples/run_sudoku.cpp
.PHONY: examples

build_test:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/*.cpp -lgtest -o build/test

test: build_test
	./build/test
.PHONY: test

clean:
	rm -rf build
