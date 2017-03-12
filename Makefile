CXXFLAGS += -I./include
CXXFLAGS += --std=c++11
CXXFLAGS += -Wall -pedantic
CXXFLAGS += -O3
# CXXFLAGS += -g


all : bin/demo

bin/demo : include/*.h
bin/demo : src/demo.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean :
	rm -rf bin/*
