CXX ?= g++
CXXFLAGS = -Iinclude -std=c++11 -pedantic -Wall -O2

LIB_SRC = src/*.cpp

all: examples

examples:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/sudoku $(LIB_SRC) example/sudoku/*.cpp
	$(CXX) $(CXXFLAGS) -o build/dlx $(LIB_SRC) example/dlx/*.cpp

build_test:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/*.cpp -lgtest -o build/test
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/test_runner.cpp test/sudoku/*.cpp -lgtest -o build/test_sudoku

test: build_test
	./build/test
	./build/test_sudoku
.PHONY: test

clean:
	rm -rf build
