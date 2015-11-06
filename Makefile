CXX ?= g++

all:
	$(CXX) -std=c++11 -pedantic -Wall -O2 -o dlx *.cpp
