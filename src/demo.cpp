#include <vector>
#include "exp_shift.h"
#include "graph.h"

using exp_shift::Adjacency;
using exp_shift::Edge;
using exp_shift::Vertex;
using exp_shift::VertexDistLess;
using exp_shift::partition;

int main() {
  std::vector<Edge> es;
  std::vector<Vertex> vs;
  exp_shift::Graph<Adjacency> g(es, 0);
  partition<Adjacency, Vertex, VertexDistLess, offsetof(Vertex, pairing_node)>(
      g, vs, 0.5
  );
  return 0;
}
