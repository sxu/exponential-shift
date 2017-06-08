#ifndef GRAPH_H
#define GRAPH_H

#include "pairing_heap.h"

namespace exp_shift {

struct Edge {
  size_t v1;
  size_t v2;

  Edge() = default;

  Edge(size_t v1_, size_t v2_)
    : v1(v1_), v2(v2_)
  { }
};

struct Adjacency {
  size_t v;

  Adjacency() = default;

  Adjacency(size_t v_) : v(v_) { }

  template <typename EdgeT>
  void set(size_t u, const EdgeT& e) {
    v = u ^ e.v1 ^ e.v2;
  }
};

struct Vertex {
  // PairingNode pairing_node;
  size_t id;
  size_t parent;
  size_t ancestor;
  double shifted_dist;
  double dist;
  bool finished;

  Vertex() = default;

  void initialize(size_t i, double shift) {
    id = parent = ancestor = i;
    shifted_dist = shift;
    dist = 0;
    finished = false;
    // pairing_node.reset();
  }

  bool relax(const Vertex& v, const Adjacency& a) {
    double new_dist = v.shifted_dist + 1;
    if (shifted_dist <= new_dist) {
      return false;
    }

    dist = v.dist + 1;
    shifted_dist = new_dist;
    parent = v.id;
    ancestor = v.ancestor;

    return true;
  }

  bool operator<(const Vertex& o) const {
    return shifted_dist < o.shifted_dist;
  }
};

template <typename AdjT>
class Graph {
public:
  size_t n;
  std::vector<AdjT> adjacencies;
  std::vector<size_t> first_adj;

  Graph() {
    n = 0;
  }

  template <typename EdgeT>
  Graph(const std::vector<EdgeT>& es, size_t n_) {
    build_graph(es, n_);
  }

  template <typename EdgeT>
  void build_graph(const std::vector<EdgeT>& es, size_t n_) {
    n = n_;
    size_t m = es.size();
    std::vector<size_t> degrees(n);

    adjacencies.resize(m * 2);
    first_adj.resize(n + 1);

    for (const auto& e : es) {
      degrees[e.v1]++;
      degrees[e.v2]++;
    }

    first_adj[0] = 0;
    for (size_t i = 0; i < n - 1; i++) {
      first_adj[i + 1] = degrees[i] + first_adj[i];
    }
    first_adj[n] = m * 2;

    for (const auto& e : es) {
      adjacencies[first_adj[e.v1]++].set(e.v1, e);
      adjacencies[first_adj[e.v2]++].set(e.v2, e);
    }

    first_adj[0] = 0;
    for (size_t i = 0; i < n - 1; i++) {
      first_adj[i + 1] = degrees[i] + first_adj[i];
    }
    first_adj[n] = m * 2;
  }
};

template <typename EdgeT>
std::vector<EdgeT> grid2(size_t n, size_t m) {
  std::vector<EdgeT> es;

  es.reserve(n * (m - 1) + m * (n - 1));
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      if (i < n - 1) {
        es.emplace_back(i * m + j, (i + 1) * m + j);
      }
      if (j < m - 1) {
        es.emplace_back(i * m + j, i * m + j + 1);
      }
    }
  }

  return es;
}

}

#endif
