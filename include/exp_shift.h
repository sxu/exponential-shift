#ifndef EXP_SHIFT_H
#define EXP_SHIFT_H

#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>
#include "graph.h"
#include "pairing_heap.h"

namespace exp_shift {

template <typename VtxT, typename CmpT, size_t Offset>
void add_exponential_shifts(std::vector<VtxT>& vs,
                            PairingHeap<VtxT, CmpT, Offset>& queue,
                            double lambda) {
  size_t n = vs.size();
  std::mt19937 rng(std::random_device{}());
  std::exponential_distribution<double> exp_dist(lambda);

  for (size_t i = 0; i < n; i++) {
    vs[i].initialize(i, -exp_dist(rng));
    queue.insert(vs[i]);
  }
}

template <typename AdjT, typename VtxT, typename CmpT, size_t Offset>
void dijkstra(const Graph<AdjT>& g,
              std::vector<VtxT>& vs,
              PairingHeap<VtxT, CmpT, Offset>& queue) {
  while (queue.size() > 0) {
    const VtxT& u = queue.get_min();
    size_t uid = u.id;
    queue.delete_min();
    for (size_t i = g.first_adj[uid]; i < g.first_adj[uid + 1]; i++) {
      const AdjT& a = g.adjacencies[i];
      if (vs[a.v].relax(u, a)) {
        queue.decrease_key(vs[a.v]);
      }
    }
  }
}

template <typename VtxT>
void add_exponential_shifts(std::vector<VtxT>& vs,
                            std::vector<size_t>& q,
                            double lambda) {
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> unif(0, 1);

  std::shuffle(q.begin(), q.end(), rng);
  double sum = 0;
  for (size_t i = q.size(); i > 0; i--) {
    sum += std::log(1 - unif(rng)) / (lambda * i);
    vs[q[i - 1]].initialize(q[i - 1], sum);
  }
}

template <typename AdjT, typename VtxT, typename CmpT>
void bfs(const Graph<AdjT>& g,
         std::vector<VtxT>& vs,
         std::vector<size_t>& q1,
         std::vector<size_t>& q2,
         CmpT& less) {
  size_t i1 = 0;
  size_t i2 = 0;
  for (;;) {
    size_t u;
    while (i1 < q1.size() && vs[q1[i1]].finished) {
      i1++;
    }
    if (i1 < q1.size() && i2 < q2.size()) {
      if (less(vs[q1[i1]], vs[q2[i2]])) {
        u = q1[i1++];
      } else {
        u = q2[i2++];
      }
    } else if (i1 < q1.size()) {
      u = q1[i1++];
    } else if (i2 < q2.size()) {
      u = q2[i2++];
    } else {
      break;
    }
    vs[u].finished = true;
    for (size_t i = g.first_adj[u]; i < g.first_adj[u + 1]; i++) {
      const AdjT& a = g.adjacencies[i];
      if (vs[a.v].finished) {
        continue;
      }
      vs[a.v].relax(vs[u], a);
      vs[a.v].finished = true;
      q2.emplace_back(a.v);
    }
  }
}

template <typename AdjT, typename VtxT>
void bfs(const Graph<AdjT>& g,
         std::vector<VtxT>& vs,
         std::vector<size_t>& q1,
         std::vector<size_t>& q2) {
  std::less<VtxT> less;
  bfs(g, vs, q1, q2, less);
}

template <typename AdjT, typename VtxT>
void partition(const Graph<AdjT>& g,
               std::vector<VtxT>& vs,
               double lambda) {
  std::vector<size_t> q1(g.n);
  std::vector<size_t> q2;

  vs.resize(g.n);
  std::iota(q1.begin(), q1.end(), 0);
  add_exponential_shifts(vs, q1, lambda);
  q2.reserve(g.n);
  bfs(g, vs, q1, q2);

  // PairingHeap<VtxT, std::less<VtxT>, 0> queue;
  // add_exponential_shifts(vs, queue, lambda);
  // dijkstra(g, vs, queue);
}

}

#endif
