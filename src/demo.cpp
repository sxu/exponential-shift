#include <cmath>
#include <iostream>
#include <vector>
#include "exp_shift.h"
#include "graph.h"

using namespace exp_shift;
using std::cout;
using std::cerr;
using std::endl;

int main() {
  size_t k = 5000;
  size_t n = k * k;
  EdgeList<Edge> es = grid2<Edge>(k, k);
  Graph<Adjacency> g(es);
  std::vector<Vertex> vs(n);
  double lambda = 0.5;

  partition(g, vs, lambda);

  size_t count = 0;
  for (auto& e : es) {
    if (vs[e.v1].ancestor != vs[e.v2].ancestor) {
      count++;
    }
  }

  cout << static_cast<double>(count) / es.size()
       << " fraction of edges cut."
       << endl;

  double max_radius = -1;
  double total_radius = 0;
  for (auto & v : vs) {
    max_radius = std::max(max_radius, v.radius);
    total_radius += v.radius;
  }

  cout << "max radius: " << max_radius << ", "
       << "expected:  " << log(n) / lambda
       << endl;

  cout << "avg radius: " << total_radius / n << ", "
       << "hope:  " << 1 / lambda
       << endl;

  return 0;
}
