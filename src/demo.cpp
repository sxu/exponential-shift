#include <chrono>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "exp_shift.h"
#include "graph.h"
#include "spanner.h"

using namespace exp_shift;

class Timer {
  std::chrono::time_point<std::chrono::steady_clock> start;
  std::string opening;
  std::string closing;

public:
  Timer(const std::string& o=">>>>> ", const std::string& c="<<<<< ")
    : opening(o), closing(c) {
    start = std::chrono::steady_clock::now();
  }

  void tic(const std::string& msg="") {
    std::cout << opening << msg << std::endl;
    start = std::chrono::steady_clock::now();
  }

  void toc() {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << closing << "Finished in " << duration.count() << "s"
              << std::endl;
  }
};

Timer timer;

void parition_demo(const std::vector<Edge>& es,
                   size_t n,
                   double lambda) {
  std::cout << "======== Exponential Start Time Partitioning ========"
            << std::endl;

  Graph<Adjacency> g(es, n);
  std::vector<Vertex> vs(n);

  timer.tic("Partitioning...");
  partition(g, vs, lambda);
  timer.toc();

  size_t count = 0;
  for (auto& e : es) {
    if (vs[e.v1].ancestor != vs[e.v2].ancestor) {
      count++;
    }
  }

  std::cout << "lambda: " << lambda << std::endl;
  std::cout << static_cast<double>(count) / es.size()
            << " fraction of edges cut."
            << std::endl;

  double max_radius = -1;
  double total_radius = 0;
  for (auto & v : vs) {
    max_radius = std::max(max_radius, v.dist);
    total_radius += v.dist;
  }

  std::cout << "max dist: " << max_radius << ", "
            << "expected:  " << log(n) / lambda
            << std::endl;
  std::cout << "avg dist: " << total_radius / n << ", "
            << "1 / lambda = " << 1 / lambda
            << std::endl;
}

void unweighted_spanner_demo(const std::vector<Edge>& es,
                             size_t n,
                             double k) {
  std::cout << "======== Unweighted Spanner ========"
            << std::endl;

  std::vector<Vertex> vs(n);
  std::unordered_set<MyPair> cluster_pairs;
  std::vector<Edge> spanner;

  timer.tic("Finding spanner...");
  spanner = unweighted_spanner(es, n, k, vs, cluster_pairs);
  timer.toc();

  double max_stretch = -1;
  double total_stretch = 0;

  size_t i;

  for (i = 0; i < spanner.size(); i++) {
    size_t u = spanner[i].v1;
    size_t v = spanner[i].v2;
    if (vs[u].ancestor != vs[v].ancestor) {
      break;
    }
  }

  std::unordered_map<MyPair, double> center_dist;

  for (; i < spanner.size(); i++) {
    size_t u = spanner[i].v1;
    size_t v = spanner[i].v2;
    MyPair p(vs[u].ancestor, vs[v].ancestor);
    center_dist[p] = vs[u].dist + vs[v].dist + 1;
  }

  for (auto& e : es) {
    size_t u = e.v1;
    size_t v = e.v2;

    if (vs[u].parent == v || vs[v].parent == u) {
      continue;
    }

    double s = vs[u].dist + vs[v].dist;
    size_t a1 = vs[e.v1].ancestor;
    size_t a2 = vs[e.v2].ancestor;
    if (a1 != a2) {
      s += center_dist.at(MyPair(a1, a2));
    }

    total_stretch += s;
    if (s > max_stretch) {
      max_stretch = s;
    }
  }

  std::cout << "spanner density: " << static_cast<double>(spanner.size()) / n
            << std::endl;
  std::cout << "target stretch: " << k << std::endl;
  std::cout << "average stretch: "
            << total_stretch / (es.size() - spanner.size())
            << std::endl;
  std::cout << "max stretch: " << max_stretch << std::endl;
}

int main() {
  size_t k = 2000;
  size_t n = k * k;
  double lambda = 0.5;
  std::vector<Edge> es = grid2<Edge>(k, k);

  parition_demo(es, n, lambda);
  unweighted_spanner_demo(es, n, 4 * log(n));

  return 0;
}
