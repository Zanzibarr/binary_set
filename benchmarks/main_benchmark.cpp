#include <benchmark/benchmark.h>

#include <algorithm>  // For std::generate
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include "../binary_set.hxx"  // Include the binary_set header

// --- Helper Functions for generating test data ---
std::vector<unsigned int> generate_random_elements(unsigned int count, unsigned int max_value) {
    std::vector<unsigned int> elements;
    elements.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, max_value - 1);
    for (unsigned int i = 0; i < count; ++i) {
        elements.push_back(distrib(gen));
    }
    return elements;
}

// --- Fixtures ---

// Fixture for benchmarks that need a binary_set and other containers
class ContainerFixture : public benchmark::Fixture {
   public:
    void SetUp(const ::benchmark::State& state) override {
        capacity = state.range(0);
        // Ensure capacity is at least 1 for valid binary_set construction in some cases
        if (capacity == 0) capacity = 1;

        // For operations like remove/contains, we need pre-filled data
        fill_elements.clear();
        fill_elements.reserve(capacity);
        for (unsigned int i = 0; i < capacity; ++i) {
            fill_elements.push_back(i);
        }

        // Generate a set of random elements within capacity for sparse filling/testing
        num_random_elements = capacity / 2;  // Test with half-filled sets
        if (num_random_elements == 0 && capacity > 0) num_random_elements = 1;
        random_elements = generate_random_elements(num_random_elements, capacity);
    }

    void TearDown(const ::benchmark::State& state) override {
        // No special tear down needed
    }

   protected:
    unsigned int capacity;
    std::vector<unsigned int> fill_elements;
    std::vector<unsigned int> random_elements;
    unsigned int num_random_elements;
};

// --- Benchmarks for Add operations (already present, but updated for new fixture) ---

BENCHMARK_DEFINE_F(ContainerFixture, AddBinarySet)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        binary_set bs(capacity);  // Construct inside timing loop to measure full setup
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            bs.add(i);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, AddBinarySet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, AddStdSet)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::set<unsigned int> s;
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            s.insert(i);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, AddStdSet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, AddVectorBool)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<bool> v(capacity, false);
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            v[i] = true;
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, AddVectorBool)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, AddUnorderedSet)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::unordered_set<unsigned int> us;
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            us.insert(i);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, AddUnorderedSet)->Range(8, 8 << 10);

// --- Benchmarks for Remove operations ---

BENCHMARK_DEFINE_F(ContainerFixture, RemoveBinarySet)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        binary_set bs(capacity);
        for (unsigned int i : fill_elements) bs.add(i);  // Fill it up
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            bs.remove(i);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, RemoveBinarySet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, RemoveStdSet)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::set<unsigned int> s;
        for (unsigned int i : fill_elements) s.insert(i);
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            s.erase(i);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, RemoveStdSet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, RemoveVectorBool)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<bool> v(capacity, false);
        for (unsigned int i = 0; i < capacity; ++i) v[i] = true;
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            v[i] = false;  // "Remove" by setting to false
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, RemoveVectorBool)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, RemoveUnorderedSet)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        std::unordered_set<unsigned int> us;
        for (unsigned int i : fill_elements) us.insert(i);
        state.ResumeTiming();
        for (unsigned int i = 0; i < capacity; ++i) {
            us.erase(i);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, RemoveUnorderedSet)->Range(8, 8 << 10);

// --- Benchmarks for Contains operations (Hit - element exists) ---

BENCHMARK_DEFINE_F(ContainerFixture, ContainsBinarySetHit)(benchmark::State& state) {
    binary_set bs(capacity);
    for (unsigned int i : fill_elements) bs.add(i);  // Fully filled
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(bs.contains(i));
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsBinarySetHit)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, ContainsStdSetHit)(benchmark::State& state) {
    std::set<unsigned int> s;
    for (unsigned int i : fill_elements) s.insert(i);
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(s.count(i));
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsStdSetHit)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, ContainsVectorBoolHit)(benchmark::State& state) {
    std::vector<bool> v(capacity, false);
    for (unsigned int i = 0; i < capacity; ++i) v[i] = true;
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(v[i]);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsVectorBoolHit)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, ContainsUnorderedSetHit)(benchmark::State& state) {
    std::unordered_set<unsigned int> us;
    for (unsigned int i : fill_elements) us.insert(i);
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(us.count(i));
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsUnorderedSetHit)->Range(8, 8 << 10);

// --- Benchmarks for Contains operations (Miss - element does not exist) ---

BENCHMARK_DEFINE_F(ContainerFixture, ContainsBinarySetMiss)(benchmark::State& state) {
    binary_set bs(capacity);  // Empty set
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(bs.contains(i));
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsBinarySetMiss)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, ContainsStdSetMiss)(benchmark::State& state) {
    std::set<unsigned int> s;  // Empty set
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(s.count(i));
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsStdSetMiss)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, ContainsVectorBoolMiss)(benchmark::State& state) {
    std::vector<bool> v(capacity, false);  // Empty (all false)
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(v[i]);
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsVectorBoolMiss)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, ContainsUnorderedSetMiss)(benchmark::State& state) {
    std::unordered_set<unsigned int> us;  // Empty set
    for (auto _ : state) {
        for (unsigned int i = 0; i < capacity; ++i) {
            benchmark::DoNotOptimize(us.count(i));
        }
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ContainsUnorderedSetMiss)->Range(8, 8 << 10);

// --- Benchmarks for Set Operations ---

// Helper to create a half-filled binary_set
binary_set create_half_filled_binary_set(unsigned int capacity) {
    binary_set bs(capacity);
    for (unsigned int i = 0; i < capacity; i += 2) {
        bs.add(i);
    }
    return bs;
}

// Helper to create a half-filled std::set
std::set<unsigned int> create_half_filled_std_set(unsigned int capacity) {
    std::set<unsigned int> s;
    for (unsigned int i = 0; i < capacity; i += 2) {
        s.insert(i);
    }
    return s;
}

// Helper to create a half-filled std::unordered_set
std::unordered_set<unsigned int> create_half_filled_unordered_set(unsigned int capacity) {
    std::unordered_set<unsigned int> us;
    for (unsigned int i = 0; i < capacity; i += 2) {
        us.insert(i);
    }
    return us;
}

// Union
BENCHMARK_DEFINE_F(ContainerFixture, UnionBinarySet)(benchmark::State& state) {
    binary_set bs1 = create_half_filled_binary_set(capacity);
    binary_set bs2(capacity);
    for (unsigned int i = 1; i < capacity; i += 2) bs2.add(i);  // Other half
    for (auto _ : state) {
        benchmark::DoNotOptimize(bs1 | bs2);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, UnionBinarySet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, UnionStdSet)(benchmark::State& state) {
    std::set<unsigned int> s1 = create_half_filled_std_set(capacity);
    std::set<unsigned int> s2;
    for (unsigned int i = 1; i < capacity; i += 2) s2.insert(i);
    for (auto _ : state) {
        std::set<unsigned int> result;
        std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(result, result.begin()));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, UnionStdSet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, UnionUnorderedSet)(benchmark::State& state) {
    std::unordered_set<unsigned int> us1 = create_half_filled_unordered_set(capacity);
    std::unordered_set<unsigned int> us2;
    for (unsigned int i = 1; i < capacity; i += 2) us2.insert(i);
    for (auto _ : state) {
        std::unordered_set<unsigned int> result = us1;
        result.insert(us2.begin(), us2.end());
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, UnionUnorderedSet)->Range(8, 8 << 10);

// Intersection
BENCHMARK_DEFINE_F(ContainerFixture, IntersectionBinarySet)(benchmark::State& state) {
    binary_set bs1 = create_half_filled_binary_set(capacity);
    binary_set bs2 = create_half_filled_binary_set(capacity);  // Same elements for intersection
    for (auto _ : state) {
        benchmark::DoNotOptimize(bs1 & bs2);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, IntersectionBinarySet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, IntersectionStdSet)(benchmark::State& state) {
    std::set<unsigned int> s1 = create_half_filled_std_set(capacity);
    std::set<unsigned int> s2 = create_half_filled_std_set(capacity);
    for (auto _ : state) {
        std::set<unsigned int> result;
        std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(result, result.begin()));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, IntersectionStdSet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, IntersectionUnorderedSet)(benchmark::State& state) {
    std::unordered_set<unsigned int> us1 = create_half_filled_unordered_set(capacity);
    std::unordered_set<unsigned int> us2 = create_half_filled_unordered_set(capacity);
    for (auto _ : state) {
        std::unordered_set<unsigned int> result;
        for (unsigned int val : us1) {
            if (us2.count(val)) {
                result.insert(val);
            }
        }
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, IntersectionUnorderedSet)->Range(8, 8 << 10);

// Difference (A - B)
BENCHMARK_DEFINE_F(ContainerFixture, DifferenceBinarySet)(benchmark::State& state) {
    binary_set bs1(capacity);
    for (unsigned int i = 0; i < capacity; ++i) bs1.add(i);  // Full set
    binary_set bs2 = create_half_filled_binary_set(capacity);
    for (auto _ : state) {
        benchmark::DoNotOptimize(bs1 - bs2);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, DifferenceBinarySet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, DifferenceStdSet)(benchmark::State& state) {
    std::set<unsigned int> s1;
    for (unsigned int i = 0; i < capacity; ++i) s1.insert(i);
    std::set<unsigned int> s2 = create_half_filled_std_set(capacity);
    for (auto _ : state) {
        std::set<unsigned int> result;
        std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(result, result.begin()));
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, DifferenceStdSet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, DifferenceUnorderedSet)(benchmark::State& state) {
    std::unordered_set<unsigned int> us1;
    for (unsigned int i = 0; i < capacity; ++i) us1.insert(i);
    std::unordered_set<unsigned int> us2 = create_half_filled_unordered_set(capacity);
    for (auto _ : state) {
        std::unordered_set<unsigned int> result;
        for (unsigned int val : us1) {
            if (us2.find(val) == us2.end()) {
                result.insert(val);
            }
        }
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, DifferenceUnorderedSet)->Range(8, 8 << 10);

// Complement
BENCHMARK_DEFINE_F(ContainerFixture, ComplementBinarySet)(benchmark::State& state) {
    binary_set bs1 = create_half_filled_binary_set(capacity);
    for (auto _ : state) {
        benchmark::DoNotOptimize(!bs1);
    }
}
BENCHMARK_REGISTER_F(ContainerFixture, ComplementBinarySet)->Range(8, 8 << 10);

// --- Benchmarks for Iteration ---

BENCHMARK_DEFINE_F(ContainerFixture, IterateBinarySet)(benchmark::State& state) {
    binary_set bs(capacity);
    for (unsigned int i : random_elements) bs.add(i);  // Sparsely filled
    unsigned int count = 0;
    for (auto _ : state) {
        count = 0;
        for (unsigned int elem : bs) {
            benchmark::DoNotOptimize(elem);
            count++;
        }
    }
    benchmark::DoNotOptimize(count);  // Keep count alive
}
BENCHMARK_REGISTER_F(ContainerFixture, IterateBinarySet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, IterateStdSet)(benchmark::State& state) {
    std::set<unsigned int> s;
    for (unsigned int i : random_elements) s.insert(i);
    unsigned int count = 0;
    for (auto _ : state) {
        count = 0;
        for (unsigned int elem : s) {
            benchmark::DoNotOptimize(elem);
            count++;
        }
    }
    benchmark::DoNotOptimize(count);
}
BENCHMARK_REGISTER_F(ContainerFixture, IterateStdSet)->Range(8, 8 << 10);

BENCHMARK_DEFINE_F(ContainerFixture, IterateUnorderedSet)(benchmark::State& state) {
    std::unordered_set<unsigned int> us;
    for (unsigned int i : random_elements) us.insert(i);
    unsigned int count = 0;
    for (auto _ : state) {
        count = 0;
        for (unsigned int elem : us) {
            benchmark::DoNotOptimize(elem);
            count++;
        }
    }
    benchmark::DoNotOptimize(count);
}
BENCHMARK_REGISTER_F(ContainerFixture, IterateUnorderedSet)->Range(8, 8 << 10);

// Main entry point for Google Benchmark
BENCHMARK_MAIN();
