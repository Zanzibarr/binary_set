# Benchmarks: BinarySet vs. Standard Containers

This document presents a comprehensive performance and space efficiency comparison of the `binary_set` data structure against several standard C++ library containers: `std::set<unsigned int>`, `std::unordered_set<unsigned int>`, and `std::vector<bool>`.

The goal is to identify use cases where `binary_set` excels and where other containers might be more suitable.

---

## Performance Results

The benchmarks measure the time (in nanoseconds) and CPU usage (in nanoseconds) taken to perform various operations across different `capacity` (input sizes). Lower values for `Time` and `CPU` indicate better performance.

**Benchmark Environment:**
```
2025-12-08T15:34:55+01:00
Running ./build/benchmarks/run_benchmarks
Run on (12 X 4600.05 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 0.32, 0.73, 0.81
```

### 1. Add Operation (Sequential Insertion)

*   **Description:** Measures the time taken to sequentially add `capacity` elements (from 0 to `capacity`-1) into an initially empty container.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 213/214 | 297/299 | 862/864 | 5427/5430 | 10632/10635 |
| `std::set`           | 343/345 | 1844/1847 | 14608/14615 | 156388/156427 | 376291/376332 |
| `std::vector<bool>`  | 198/199 | 274/275 | 680/682 | 3927/3930 | 7620/7625 |
| `std::unordered_set` | 402/404 | 1765/1768 | 14301/14313 | 117115/117150 | 235193/235226 |


### 2. Remove Operation (Sequential Deletion)

*   **Description:** Measures the time taken to sequentially remove `capacity` elements (from 0 to `capacity`-1) from a fully populated container.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 209/211 | 290/290 | 873/875 | 5535/5536 | 10760/10761 |
| `std::set`           | 327/329 | 1404/1406 | 10858/10860 | 97928/97922 | 202844/202822 |
| `std::vector<bool>`  | 203/204 | 261/263 | 694/696 | 4218/4219 | 7980/7981 |
| `std::unordered_set` | 281/282 | 1115/1118 | 6412/6412 | 49554/49549 | 133172/133078 |


### 3. Contains Operation (Element Existence Check)

#### 3.1. Contains Hit (Element Exists)

*   **Description:** Measures the time taken to check for the existence of `capacity` elements, all of which are known to be present in the container.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 12.5/12.5 | 82.9/82.9 | 690/690 | 5848/5848 | 11625/11625 |
| `std::set`           | 22.8/22.8 | 328/328 | 3337/3337 | 133437/133430 | 295298/295294 |
| `std::vector<bool>`  | 12.9/12.9 | 96.9/96.9 | 815/815 | 6582/6582 | 13209/13209 |
| `std::unordered_set` | 19.6/19.6 | 157/157 | 1204/1204 | 9846/9845 | 19362/19362 |


#### 3.2. Contains Miss (Element Does Not Exist)

*   **Description:** Measures the time taken to check for the existence of `capacity` elements, all of which are known to be absent from the container.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 10.9/10.9 | 80.7/80.7 | 658/658 | 5727/5726 | 11407/11407 |
| `std::set`           | 13.0/13.0 | 91.9/91.9 | 773/773 | 6443/6443 | 12980/12980 |
| `std::vector<bool>`  | 12.4/12.4 | 93.3/93.2 | 789/789 | 6382/6381 | 12759/12758 |
| `std::unordered_set` | 11.1/11.1 | 92.5/92.4 | 743/743 | 6054/6054 | 12546/12546 |


### 4. Set Operations (Set Algebra)

#### 4.1. Union (A | B)

*   **Description:** Measures the time taken to compute the union of two containers.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 13.0/13.0 | 23.7/23.7 | 126/126 | 838/838 | 1650/1650 |
| `std::set`           | 183/183 | 1727/1726 | 15196/15196 | 142333/142323 | 303985/303973 |
| `std::unordered_set` | 120/120 | 1199/1199 | 13337/13336 | 107211/107200 | 214797/214787 |


#### 4.2. Intersection (A & B)

*   **Description:** Measures the time taken to compute the intersection of two containers.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 12.6/12.6 | 23.3/23.3 | 103/103 | 643/643 | 1262/1262 |
| `std::set`           | 87.0/87.0 | 818/818 | 7559/7558 | 71504/71500 | 150639/150629 |
| `std::unordered_set` | 103/103 | 864/864 | 8343/8343 | 67306/67301 | 133765/133755 |


#### 4.3. Difference (A - B)

*   **Description:** Measures the time taken to compute the set difference (elements in A but not in B).

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 12.7/12.7 | 24.2/24.2 | 107/107 | 663/663 | 1301/1301 |
| `std::set`           | 97.0/97.0 | 912/912 | 8321/8321 | 79289/79286 | 164438/164425 |
| `std::unordered_set` | 116/116 | 948/948 | 9709/9708 | 76988/76983 | 153010/152998 |


#### 4.4. Complement (Unary NOT)

*   **Description:** Measures the time taken to compute the complement of a `binary_set` (elements not present in the set within its `capacity`). This operation is unique to bitset-like structures.

| Container Type | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`   | 17.5/17.5 | 27.0/27.0 | 106/106 | 618/618 | 1207/1207 |


### 5. Iteration

*   **Description:** Measures the time taken to iterate over all elements (half-filled set with random elements) present in the container.

| Container Type       | 8 (Time/CPU) | 64 (Time/CPU) | 512 (Time/CPU) | 4096 (Time/CPU) | 8192 (Time/CPU) |
| :------------------- | :----------- | :------------ | :------------- | :-------------- | :-------------- |
| `binary_set`         | 9.28/9.28 | 65.4/65.4 | 504/504 | 3957/3957 | 8260/8260 |
| `std::set`           | 5.72/5.72 | 58.8/58.8 | 453/452 | 6279/6279 | 14662/14661 |
| `std::unordered_set` | 5.61/5.61 | 37.9/37.9 | 255/255 | 2446/2446 | 6330/6330 |




## Space Efficiency (Theoretical Analysis)

The `binary_set` is fundamentally designed for space efficiency, storing each potential element as a single bit. Here's a theoretical analysis of memory usage, highlighting its compact nature:

Let `N` be the `capacity` (the maximum possible element value + 1).

1.  **`binary_set`:**
    *   **Underlying storage:** `std::vector<unsigned char>`.
    *   **Memory usage:** `ceil(N / 8.0)` bytes for the bit storage itself, plus a small constant overhead for the `std::vector` object.
    *   **Example (N = 8192):** `ceil(8192 / 8.0) = 1024` bytes (1 KB).

2.  **`std::vector<bool>`:**
    *   **Underlying storage:** Specialized template, typically implemented as a packed bitset.
    *   **Memory usage:** `ceil(N / 8.0)` bytes for the bit storage, plus small constant overhead.
    *   **Example (N = 8192):** `ceil(8192 / 8.0) = 1024` bytes (1 KB).
    *   *Note:* `binary_set` and `std::vector<bool>` are designed for similar space efficiency in terms of bit storage.

3.  **`std::set<unsigned int>`:**
    *   **Underlying storage:** Typically a Red-Black Tree. Each node stores the element, plus pointers (parent, left, right), and a color bit.
    *   **Memory usage (per element stored):** Roughly `sizeof(unsigned int)` (e.g., 4 bytes) + `3 * sizeof(pointer)` (e.g., 3 * 8 bytes on a 64-bit system) + small overhead. This is approximately `4 + 24 = 28` bytes per element stored.
    *   **Example (N = 8192, storing all N elements):** `8192 * ~28 bytes = ~229,376` bytes (~224 KB).

4.  **`std::unordered_set<unsigned int>`:**
    *   **Underlying storage:** Typically a hash table. Elements are stored in buckets, often using linked lists for collision resolution.
    *   **Memory usage (per element stored):** `sizeof(unsigned int)` + pointer overhead (for linked lists) + hash table structure (array of buckets) overhead. Can be highly variable depending on load factor and collision strategy.
    *   **Example (N = 8192, storing all N elements):** `8192 * ~20-30 bytes = ~160 KB - 240 KB` (rough estimate).

### Space Efficiency Comparison for N = 8192 (storing all N elements):

*   **`binary_set`:** ~1 KB
*   **`std::vector<bool>`:** ~1 KB
*   **`std::set<unsigned int>`:** ~224 KB
*   **`std::unordered_set<unsigned int>`:** ~160-240 KB

### Key Takeaways for Space Efficiency:

`binary_set` (and `std::vector<bool>`) offer dramatically superior space efficiency compared to `std::set` and `std::unordered_set` when representing dense sets of non-negative integers within a fixed, contiguous range. This makes `binary_set` particularly well-suited for memory-constrained applications or scenarios where a compact bitmask representation is desirable. However, for very sparse sets with a massive `capacity`, storing only the actual elements in `std::set` or `std::unordered_set` might become more efficient.

---

## Conclusion: Optimal Use Cases for `binary_set`

`binary_set` is an excellent choice for scenarios where you need a highly optimized, compact representation of a set of non-negative integers within a known, finite range, and especially when:

*   **Extensive Set Algebra (Union, Intersection, Difference, Complement) is required.**
*   **Memory footprint is a primary concern.**

Its performance for these operations significantly surpasses standard library containers. For applications like feature flags, bitmasks, or compact set representations, `binary_set` offers substantial performance and space advantages.

However, for very sparse sets with extremely large capacities, or when fast iteration over sparse elements is the primary requirement, `std::set` or `std::unordered_set` might be more appropriate due to their dynamic memory allocation and direct element storage.
