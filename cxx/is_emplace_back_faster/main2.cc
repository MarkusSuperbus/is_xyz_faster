#include <benchmark/benchmark.h>
#include <vector>
#include <memory>

#define RANGE_LO (8 << 10)
#define RANGE_HI (8 << 10)
template <typename ComplexType>
static void BM_emplaceBack(benchmark::State& state) {
    std::vector<ComplexType> v;
    v.reserve(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        v.emplace_back(a);
    }
}

template <typename ComplexType>
static void BM_pushBack(benchmark::State& state) {
    std::vector<ComplexType> v;
    v.reserve(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        v.push_back(ComplexType(a));
    }
}

template <typename ComplexType>
static void BM_pushBackMove(benchmark::State& state) {
    std::vector<ComplexType> v;
    v.reserve(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        auto value = ComplexType(a);
        v.push_back(std::move(value));
    }
}

template <typename ComplexType>
static void BM_pushBackCopy(benchmark::State& state) {
    std::vector<ComplexType> v;
    v.reserve(state.iterations());
    int a = state.range(0);
    for (auto _: state) {
        auto value = ComplexType(a);
        v.push_back(value);
    }
}

template <typename ComplexType>
static void BM_baseline(benchmark::State& state) {
    int a = state.range(0);
    std::vector<ComplexType> v;
    v.reserve(state.iterations());
    for (auto _: state) {
        volatile auto value = ComplexType(a);
    }
}

using BaseT = int;

BENCHMARK(BM_baseline<BaseT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<BaseT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<BaseT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<BaseT>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<BaseT>)->Range(RANGE_LO, RANGE_HI);

struct SharedPointer {
    std::shared_ptr<int> a;
    SharedPointer(int aa) : a(std::make_shared<int>(aa)) {}
    SharedPointer(): a(0) {};
};
BENCHMARK(BM_baseline<SharedPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<SharedPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<SharedPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<SharedPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<SharedPointer>)->Range(RANGE_LO, RANGE_HI);

struct UniquePointer{
    std::unique_ptr<int> a;
    UniquePointer(int aa) : a(std::make_unique<int>(aa)) {}
    UniquePointer(): a(nullptr) {};
};
BENCHMARK(BM_baseline<UniquePointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<UniquePointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<UniquePointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<UniquePointer>)->Range(RANGE_LO, RANGE_HI);
// BENCHMARK(BM_pushBackCopy<UniquePair>)->Range(RANGE_LO, RANGE_HI);

// Non-owning
struct RawPointer{
    BaseT* base;
    mutable bool owning;
    RawPointer(int a):base{new BaseT(a)}, owning(true) {
        
    }
    RawPointer(): base(nullptr), owning(false) {}
    RawPointer(const RawPointer& other): base(other.base), owning(true) {
        other.owning = false;
    }
    RawPointer(RawPointer&& other): base(other.base), owning(true) {
        other.owning = false;
    }
    ~RawPointer() {
        if (base != nullptr && owning) {
            delete base;
        }
    }
};

BENCHMARK(BM_baseline<RawPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_emplaceBack<RawPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBack<RawPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackMove<RawPointer>)->Range(RANGE_LO, RANGE_HI);
BENCHMARK(BM_pushBackCopy<RawPointer>)->Range(RANGE_LO, RANGE_HI);

BENCHMARK_MAIN();
