#include "ISearchTree.h"
#include "SearchTree.h"
#include "SortedAssociationVector.h"
#include "StandardMapWrapper.h"
#include <benchmark/benchmark.h>
#include <cassert>
#include <cstdint>

using datastructures::SearchTree;
using datastructures::SortedAssociationVector;

template <ISearchTree<int64_t, int64_t> ImplT>
void BM_linear_find(::benchmark::State &state) {
  ImplT tree;
  const int64_t limit = state.range(0);
  size_t lookupKey = 0;
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

    [[maybe_unused]] auto a = tree.find(lookupKey);
    assert(a && lookupKey == size_t(a->get()));
    lookupKey++;
    lookupKey %= limit;
  }
  state.SetComplexityN(limit);
};

using IntSearchTree = SearchTree<int64_t, int64_t>;
using IntStandardMapWrapper = StandardMapWrapper<int64_t, int64_t>;
using IntSortedAssociationVector = SortedAssociationVector<int64_t, int64_t>;

BENCHMARK(BM_linear_find<IntSearchTree>)
    ->Range(1, 1 << 12)
    ->Complexity(benchmark::BigO::oAuto);

BENCHMARK(BM_linear_find<IntStandardMapWrapper>)
    ->Range(1, 1 << 12)
    ->Complexity(benchmark::BigO::oAuto);

BENCHMARK(BM_linear_find<IntSortedAssociationVector>)
    ->Range(1, 1 << 12)
    ->Complexity(benchmark::BigO::oAuto);

BENCHMARK_MAIN();
