CXX ?= g++
CXXFLAGS = -Iinclude -std=c++11 -pedantic -Wall -O2

LIB_SRC = src/*.cpp

all: examples

examples:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/dlx $(LIB_SRC) example/dlx/*.cpp
	$(CXX) $(CXXFLAGS) -o build/sudoku $(LIB_SRC) example/sudoku/*.cpp
	$(CXX) $(CXXFLAGS) -o build/langford $(LIB_SRC) example/langford/*.cpp
	$(CXX) $(CXXFLAGS) -o build/nqueens $(LIB_SRC) example/nqueens/*.cpp

build_test:
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/*.cpp -lgtest -o build/test
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/test_runner.cpp test/sudoku/*.cpp -lgtest -o build/test_sudoku
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/test_runner.cpp test/langford/*.cpp -lgtest -o build/test_langford
	$(CXX) $(CXXFLAGS) $(LIB_SRC) test/test_runner.cpp test/nqueens/*.cpp -lgtest -o build/test_nqueens

test: build_test
	./build/test
	./build/test_sudoku
	./build/test_langford
	./build/test_nqueens
.PHONY: test

clean:
	rm -rf build
