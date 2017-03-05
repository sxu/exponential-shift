#ifndef EXP_SHIFT_H
#define EXP_SHIFT_H

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
  std::vector<double> shifts(n);
  double max_shift = -1;
  std::mt19937 rng(std::random_device{}());
  std::exponential_distribution<double> exp_dist(lambda);

  for (size_t i = 0; i < n; i++) {
    shifts[i] = exp_dist(rng);
    if (shifts[i] > max_shift) {
      max_shift = shifts[i];
    }
  }

  for (size_t i = 0; i < n; i++) {
    vs[i].initialize(i, max_shift - shifts[i] + 1);
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

template <typename AdjT, typename VtxT, typename CmpT, size_t Offset>
void partition(const Graph<AdjT>& g,
               std::vector<VtxT>& vs,
               double lambda) {
  PairingHeap<VtxT, CmpT, Offset> queue;
  add_exponential_shifts(vs, queue, lambda);
  dijkstra(g, vs, queue);
}

}

#endif
