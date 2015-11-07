CXX ?= g++
CXXFLAGS = -Iinclude -std=c++11 -pedantic -Wall -O2 -o dlx

all:
	$(CXX) $(CXXFLAGS) src/*.cpp
