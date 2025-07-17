#include "ISearchTree.h"
#include "SearchTree.h"
#include "StandardMapWrapper.h"
#include <benchmark/benchmark.h>
#include <cstdint>

using datastructures::SearchTree;

template <ISearchTree<int64_t, int64_t> ImplT>
void BM_linear_find(::benchmark::State &state) {
  ImplT tree;
  const int64_t limit = state.range(0);
  size_t i = 0;
  bool first = true;
  for (auto _ : state) {
    if (first) {
      state.PauseTiming();
      for (int64_t i = 0; i < limit; ++i) {
        tree.insert(int64_t(i), int64_t(i));
      }
      first = false;
      state.ResumeTiming();
    }

    [[maybe_unused]] volatile auto a = tree.find(i);
    i++;
    i %= limit;
  }
  state.SetComplexityN(limit);
};

using IntSearchTree = SearchTree<int64_t, int64_t>;
using IntStandardMapWrapper = StandardMapWrapper<int64_t, int64_t>;

BENCHMARK(BM_linear_find<IntSearchTree>)
    ->Range(1, 1 << 12)
    ->Complexity(benchmark::BigO::oAuto);

auto stuff(::benchmark::State &state) {
  return BM_linear_find<IntStandardMapWrapper>(state);
}

BENCHMARK(BM_linear_find<IntStandardMapWrapper>)->Range(1, 1 << 12)->Complexity(benchmark::BigO::oAuto);

BENCHMARK_MAIN();
