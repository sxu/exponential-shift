#include <cmath>
#include <unordered_set>
#include <vector>
#include "exp_shift.h"
#include "graph.h"

namespace exp_shift {

struct MyPair {
  size_t i1;
  size_t i2;

  MyPair(size_t i1_, size_t i2_) {
    i1 = std::min(i1_, i2_);
    i2 = std::max(i1_, i2_);
  }

  bool operator==(const MyPair& o) const {
    return i1 == o.i1 && i2 == o.i2;
  }
};

}

namespace std {

template<> struct hash<exp_shift::MyPair> {
  typedef exp_shift::MyPair argument_type;
  typedef size_t result_type;

  result_type operator()(argument_type const& p) const {
    result_type h = p.i1;
    h ^= p.i2 + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

}

namespace exp_shift {

template <typename EdgeT>
std::vector<EdgeT> unweighted_spanner(const std::vector<EdgeT>& es,
                                      size_t n,
                                      double k,
                                      std::vector<Vertex>& vs,
                                      std::unordered_set<MyPair> pairs) {
  Graph<Adjacency> g(es, n);
  std::vector<Edge> spanner;
  double lambda = 4 * log(n) / k;

  partition(g, vs, lambda);

  for (auto& v : vs) {
    if (v.parent != v.id) {
      spanner.emplace_back(v.id, v.parent);
    }
  }

  for (auto& e : es) {
    size_t a1 = vs[e.v1].ancestor;
    size_t a2 = vs[e.v2].ancestor;
    if (a1 != a2) {
      MyPair p(a1, a2);
      if (pairs.find(p) == pairs.end()) {
        pairs.insert(p);
        spanner.emplace_back(e);
      }
    }
  }

  return spanner;
}

template <typename EdgeT>
std::vector<EdgeT> unweighted_spanner(const std::vector<EdgeT>& es,
                                      size_t n,
                                      double k) {
  std::vector<Vertex> vs(n);
  std::unordered_set<MyPair> pairs;
  return unweighted_spanner(es, n, k, vs, pairs);
}

}
