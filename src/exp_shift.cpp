#include <cstddef>
#include <random>
#include <vector>
#include "exp_shift.h"
#include "pairing_heap.h"

namespace exp_shift {

struct VertexDistLess {
  bool operator()(const Vertex& u, const Vertex& v) {
    return u.dist < v.dist;
  }
};

typedef PairingHeap<Vertex, offsetof(Vertex, pairing_node), VertexDistLess>
DijkstraQueue;

static void add_exp_shifts(std::vector<Vertex>& vs,
                           DijkstraQueue& queue,
                           double lambda) {
  size_t n = vs.size();
  std::vector<double> shifts(n);
  double max_shift = -1;
  std::mt19937 rng(std::random_device{}());
  std::exponential_distribution<double> exp_dist(lambda);

  for (size_t i = 0; i < n; i++) {
    shifts[i] = exp_shift(rng);
    if (shifts[i] > max_shift) {
      max_shift = shifts[i];
    }
  }

  for (size_t i = 0; i < n; i++) {
    vs[i].initialize(i);
    vs[i].dist = max_shift - shifts[i] + 1;
    queue.insert(vs[i]);
  }
}

static void dijkstra(const Graph& g,
                     std::vector<Vertex>& vs,
                     DijkstraQueue& queue) {
  while (queue.size() > 0) {
    const Vertex& u = queue.get_min();
    size_t uid = u.id;
    queue.delete_min();
    for (size_t i = g.first_adj[uid]; i < g.first_adj[uid + 1]; i++) {
      const Adjacency& a = g.adjacencies[i];
      double new_dist = u.dist + 1;
      if (vs[a.v].dist <= new_dist) {
        continue;
      }
      vs[a.v].dist = new_dist;
      vs[a.v].ancestor = 
    }
  }
}

void partition(const std::vector<Edge>& es,
               std::vector<Vertex>& vs,
               double lambda) {

}

}
