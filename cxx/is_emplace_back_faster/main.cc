#include <benchmark/benchmark.h>
#include <vector>
#include <memory>

#define RANGE_LO (8 << 10)
#define RANGE_HI (8 << 10)
template <typename ComplexType>
static void BM_emplaceBack(benchmark::State& state) {
    std::vector<ComplexType> v(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        v.emplace_back(a, 0);
    }
}

template <typename ComplexType>
static void BM_pushBack(benchmark::State& state) {
    std::vector<ComplexType> v(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        v.push_back(ComplexType(a, 0));
    }
}

template <typename ComplexType>
static void BM_pushBackMove(benchmark::State& state) {
    std::vector<ComplexType> v(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        auto value = ComplexType(a, 0);
        v.push_back(std::move(value));
    }
}

template <typename ComplexType>
static void BM_pushBackCopy(benchmark::State& state) {
    std::vector<ComplexType> v(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        auto value = ComplexType(a, 0);
        v.push_back(value);
    }
}

template <typename ComplexType>
static void BM_baseline(benchmark::State& state) {
    int a = state.range(0);
    for (auto _: state) {
        volatile auto value = ComplexType(a, 0);
    }
}

struct Pair {
    int a;
    int b;
    Pair(int aa, int bb) : a(aa), b(bb) {}
    Pair(): a(0), b(0) {};
};

BENCHMARK(BM_baseline<Pair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<Pair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<Pair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<Pair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<Pair>)->Range(RANGE_LO, RANGE_HI);


using VectorT = std::vector<int>;
BENCHMARK(BM_baseline<VectorT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<VectorT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<VectorT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<VectorT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<VectorT>)->Range(RANGE_LO, RANGE_HI);

struct SharedPair {
    std::shared_ptr<int> a;
    std::shared_ptr<int> b;
    SharedPair(int aa, int bb) : a(std::make_shared<int>(aa)), b(std::make_shared<int>(bb)) {}
    SharedPair(): a(0), b(0) {};
};
BENCHMARK(BM_baseline<SharedPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<SharedPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<SharedPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<SharedPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<SharedPair>)->Range(RANGE_LO, RANGE_HI);

struct SharedVector {
    std::shared_ptr<VectorT> vector;
    SharedVector(int a, int b): vector(std::make_shared<VectorT>(a, b)) {}
    SharedVector(): vector(0) {};
};
BENCHMARK(BM_baseline<SharedVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<SharedVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<SharedVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<SharedVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<SharedVector>)->Range(RANGE_LO, RANGE_HI);

struct UniquePair{
    std::unique_ptr<int> a;
    std::unique_ptr<int> b;
    UniquePair(int aa, int bb) : a(std::make_unique<int>(aa)), b(std::make_unique<int>(bb)) {}
    UniquePair(): a(nullptr), b(nullptr) {};
};
BENCHMARK(BM_baseline<UniquePair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<UniquePair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<UniquePair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<UniquePair>)->Range(RANGE_LO, RANGE_HI);
// BENCHMARK(BM_pushBackCopy<UniquePair>)->Range(RANGE_LO, RANGE_HI);

struct UniqueVector {
    std::unique_ptr<VectorT> vector;
    UniqueVector(int a, int b): vector(std::make_unique<VectorT>(a, b)) {}
    UniqueVector(): vector(nullptr) {};
};
BENCHMARK(BM_baseline<UniqueVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<UniqueVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<UniqueVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<UniqueVector>)->Range(RANGE_LO, RANGE_HI);
// BENCHMARK(BM_pushBackCopy<UniqueVector>)->Range(RANGE_LO, RANGE_HI);

// Non-owning
struct RawPointerVector {
    VectorT* vector;
    mutable bool owning;
    RawPointerVector(int a, int b):vector{new VectorT(a, b)}, owning(true) {
        
    }
    RawPointerVector(): vector(nullptr), owning(false) {}
    RawPointerVector(const RawPointerVector& other): vector(other.vector), owning(true) {
        other.owning = false;
    }
    RawPointerVector(RawPointerVector&& other): vector(other.vector), owning(true) {
        other.owning = false;
    }
    ~RawPointerVector() {
        if (vector != nullptr && owning) {
            delete vector;
        }
    }
};

BENCHMARK(BM_baseline<RawPointerVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<RawPointerVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<RawPointerVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<RawPointerVector>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<RawPointerVector>)->Range(RANGE_LO, RANGE_HI);

// Non-owning
struct RawPointerPair{
    Pair* pair;
    mutable bool owning;
    RawPointerPair(int a, int b):pair{new Pair(a, b)}, owning(true) {
        
    }
    RawPointerPair(): pair(nullptr), owning(false) {}
    RawPointerPair(const RawPointerPair& other): pair(other.pair), owning(true) {
        other.owning = false;
    }
    RawPointerPair(RawPointerPair&& other): pair(other.pair), owning(true) {
        other.owning = false;
    }
    ~RawPointerPair() {
        if (pair != nullptr && owning) {
            delete pair;
        }
    }
};

BENCHMARK(BM_baseline<RawPointerPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<RawPointerPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<RawPointerPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<RawPointerPair>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<RawPointerPair>)->Range(RANGE_LO, RANGE_HI);

BENCHMARK_MAIN();
