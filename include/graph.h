#ifndef GRAPH_H
#define GRAPH_H

#include "pairing_heap.h"

namespace exp_shift {

struct Edge {
  size_t v1;
  size_t v2;
  double length;
};

struct Adjacency {
  size_t v;
  double length;

  void set(size_t u, const Edge& e) {
    v = u ^ e.v1 ^ e.v2;
    length = e.length;
  }
};

struct Vertex {
  PairingNode pairing_node;
  size_t id;
  size_t parent;
  size_t ancestor;
  double dist;

  void initialize(size_t i, double d) {
    id = parent = ancestor = i;
    dist = d;
    pairing_node.reset();
  }

  bool relax(const Vertex& v, const Adjacency& a) {
    double new_dist = v.dist + a.length;

    if (dist <= new_dist) {
      return false;
    }

    dist = new_dist;
    parent = v.id;
    ancestor = v.ancestor;

    return true;
  }
};

struct VertexDistLess {
  bool operator()(const Vertex& v1, const Vertex& v2) {
    return v1.dist < v2.dist;
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

    size_t idx;
    for (const auto& e : es) {
      idx = first_adj[e.v1] + degrees[e.v1];
      adjacencies[idx].set(e.v1, e);
      first_adj[idx]++;

      idx = first_adj[e.v2] + degrees[e.v2];
      adjacencies[idx].set(e.v2, e);
      first_adj[idx]++;
    }

    first_adj[0] = 0;
    for (size_t i = 0; i < n - 1; i++) {
      first_adj[i + 1] = degrees[i] + first_adj[i];
    }
    first_adj[n] = m * 2;
  }
};

}

#endif
