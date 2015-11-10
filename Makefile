CXX ?= g++
CXXFLAGS = -Iinclude -std=c++11 -pedantic -Wall -O2

LIB_SRC = src/AlgorithmDLX.cpp src/LinkedMatrix.cpp

all:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/dlx $(LIB_SRC) src/main.cpp

clean:
	rm -rf build
