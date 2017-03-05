CXXFLAGS += -I./include
CXXFLAGS += --std=c++11
CXXFLAGS += -Wall -pedantic
CXXFLAGS += -O3
# CXXFLAGS += -g


all : bin/demo # bin/gen_cayley bin/graph_to_matrix bin/graph_to_graph

bin/demo : include/*.h
bin/demo : src/demo.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean :
	rm -rf bin/*
