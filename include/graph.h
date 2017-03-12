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

  Adjacency(size_t v_, double length_)
    : v(v_)
  { }

  void set(size_t u, const Edge& e) {
    v = u ^ e.v1 ^ e.v2;
  }
};

struct Vertex {
  // PairingNode pairing_node;
  size_t id;
  size_t parent;
  size_t ancestor;
  double dist;
  double radius;
  bool finished;

  Vertex() = default;

  void initialize(size_t i, double d) {
    id = parent = ancestor = i;
    dist = d;
    radius = 0;
    finished = false;
    // pairing_node.reset();
  }

  bool relax(const Vertex& v, const Adjacency& a) {
    double new_dist = v.dist + 1;
    if (dist <= new_dist) {
      return false;
    }

    radius = v.radius + 1;
    dist = new_dist;
    parent = v.id;
    ancestor = v.ancestor;

    return true;
  }

  bool operator<(const Vertex& o) const {
    return dist < o.dist;
  }
};

template <typename EdgeT>
struct EdgeList {
  size_t n;
  std::vector<EdgeT> edges;

  EdgeList(size_t n_ = 0) : n(n_) { }

  size_t size() const {
    return edges.size();
  }

  void push_back(const EdgeT& e) {
    edges.push_back(e);
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    edges.emplace_back(std::forward<Args>(args)...);
  }

  void clear() {
    n = 0;
    edges.clear();
  }

  void reserve(size_t m) {
    edges.reserve(m);
  }

  void resize(size_t m) {
    edges.resize(m);
  }

  EdgeT& operator[](const size_t i) {
    return edges[i];
  }

  const EdgeT& operator[](const size_t i) const {
    return edges[i];
  }

  typename std::vector<EdgeT>::iterator begin() {
    return edges.begin();
  }

  typename std::vector<EdgeT>::const_iterator begin() const {
    return edges.cbegin();
  }

  typename std::vector<EdgeT>::iterator end() {
    return edges.end();
  }

  typename std::vector<EdgeT>::const_iterator end() const {
    return edges.cend();
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
  Graph(const EdgeList<EdgeT>& es) {
    build_graph(es);
  }

  template <typename EdgeT>
  void build_graph(const EdgeList<EdgeT>& es) {
    n = es.n;
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
EdgeList<EdgeT> grid2(size_t n, size_t m) {
  EdgeList<EdgeT> es(n * m);

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
