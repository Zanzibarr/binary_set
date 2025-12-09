# binary_set.hxx - Compact Binary Set Library

A header-only C++ library providing space-efficient binary set operations using bit manipulation.  

## Build Status

[![C++ CI](https://github.com/Zanzibarr/binary_set/actions/workflows/ci.yml/badge.svg)](https://github.com/Zanzibarr/binary_set/actions/workflows/ci.yml)

## Features

- **Space Efficient**: Uses only 1 bit per potential element
- **Fast Operations**: Bitwise operations for set union, intersection, difference, and complement
- **Range-Safe**: Built-in bounds checking with meaningful error messages
- **STL-Compatible**: Provides forward iterators for range-based loops
- **Header-Only**: No compilation or linking required
- **Zero Dependencies**: Uses only C++ standard library
- **Subset Search**: Efficient tree-based structure for finding all subsets

## Index
- [Quick Start](#quick-start)
- [Classes](#classes)
- [Build Instructions](#how-to-build-the-project)
- [Unit Tests](#how-to-run-tests)
- [Benchmarks](#how-to-run-benchmarks)
- [Installation](#installation)
- [Limitations](#limitations)
- [Compiler Compatibility](#compiler-compatibility)
- [Contributing](#contributing)
- [Development](#development)
- [License](#license)
- [Other](#see-also)

## Quick Start

```cpp
#include "binary_set.hxx"
#include <iostream>     // For std::cout
#include <vector>       // For std::vector (used in sparse() implicitly, or if needed directly)
#include <numeric>      // For std::iota (useful for examples, though not in this snippet)
#include <algorithm>    // For std::sort (if iterating into vector and sorting for comparison)

int main() {
    // Create a set with capacity 16 (elements 0-15)
    binary_set bs(16);
    
    // Add elements
    bs.add(5);
    bs.add(10);
    bs.add(15);
    
    // Check membership
    if (bs.contains(5)) {
        std::cout << "Element 5 is in the set." << std::endl;
    }
    
    // Iterate over elements (ascending order)
    std::cout << "Elements in bs: ";
    for (unsigned int elem : bs) {
        std::cout << elem << " ";  // Output: 5 10 15
    }
    std::cout << std::endl;
    
    // Set operations
    binary_set bs2(16);
    bs2.add(5);
    bs2.add(7);
    
    auto intersection = bs & bs2;  // {5}
    std::cout << "Intersection (bs & bs2): " << static_cast<std::string>(intersection) << std::endl;
    
    auto union_set = bs | bs2;      // {5, 7, 10, 15}
    std::cout << "Union (bs | bs2): " << static_cast<std::string>(union_set) << std::endl;
    
    auto difference = bs - bs2;     // {10, 15}
    std::cout << "Difference (bs - bs2): " << static_cast<std::string>(difference) << std::endl;
    
    auto complement = !bs;          // {0, 1, 2, 3, 4, 6, 7, 8, 9, 11, 12, 13, 14}
    std::cout << "Complement (!bs): " << static_cast<std::string>(complement) << std::endl;
    
    return 0;
}
```

## Classes

This library defines two main classes: `binary_set` for compact binary set storage and operations, and `bs_searcher` for efficient subset searching.

### `binary_set`

A space-efficient set implementation that stores elements as bits.

#### Core Concepts & Internal Mechanism
*   Uses `std::vector<unsigned char>` to store bits, optimizing for memory.
*   Operations like `add`, `remove`, `contains` are O(1) by using direct bit manipulation.
*   Set operations (union, intersection, difference, complement) are performed efficiently with byte-wise bitwise logic.
*   Maintains an internal `size_` counter for O(1) element count, updated with Brian Kernighan's algorithm after bulk operations.

#### Constructors

```cpp
binary_set(); // Default: capacity 0. Operations on a 0-capacity set will throw `std::domain_error`.
binary_set(unsigned int capacity, bool fill=false);     // With capacity, optionally filled
```

#### Core Operations

| Method | Description | Time Complexity |
|--------|-------------|----------------|
| `add(element)` | Add element to set | O(1) |
| `remove(element)` | Remove element from set | O(1) |
| `contains(element)` | Check if element exists | O(1) |
| `operator[](element)` | Check if element exists (read-only) | O(1) |
| `clear()` | Remove all elements | O(n/8) |
| `fill()` | Add all elements | O(n/8) |
| `empty()` | Check if set is empty | O(n/8) |
| `size()` | Count elements in set | O(1) |
| `capacity()` | Get maximum capacity | O(1) |

#### Set Operations

| Operator | Description | Returns |
|----------|-------------|---------|
| `a & b` | Intersection | Elements in both sets |
| `a \| b` | Union | Elements in either set |
| `a - b` | Difference | Elements in a but not in b |
| `!a` | Complement | All elements not in a |
| `a == b` | Equality | true if sets contain same elements |
| `a != b` | Inequality | true if sets differ |

All binary operators have in-place variants (`&=`, `|=`, `-=`).

#### Advanced Methods

```cpp
bool intersects(const binary_set& other) const;     // Check if sets overlap
bool contains(const binary_set& subset) const;      // Check if other is subset
std::vector<unsigned int> sparse() const;           // Get sorted vector of elements
explicit operator std::string() const;              // String representation
```

#### Iterators

```cpp
iterator begin() const;  // First element (ascending order)
iterator end() const;    // Past-the-end iterator
```

Supports range-based for loops and standard algorithms.

### `bs_searcher`

Efficiently finds all subsets within a collection of binary sets using a tree structure.

#### Core Concepts & Internal Mechanism
*   Uses a trie-like tree where each level represents an element's presence/absence, allowing fast subset lookups.
*   `std::unique_ptr` manages tree nodes (`treenode`) for safe memory handling.
*   `add()` and `remove()` methods traverse the tree based on the `binary_set`'s bit pattern, with `remove()` including logic to prune empty branches, keeping the tree compact.
*   `find_subsets()` efficiently navigates the tree to collect identifiers of all stored sets that are subsets of a query set.

#### Constructor

```cpp
bs_searcher(unsigned int capacity);  // Create searcher for sets of given capacity
```

#### Methods

| Method | Description | Time Complexity |
|--------|-------------|----------------|
| `add(value, bs)` | Add set with identifier | O(capacity) |
| `remove(value, bs)` | Remove first matching set | O(capacity) |
| `find_subsets(bs)` | Find all stored subsets of bs | O(capacity × matches) |

#### Example

```cpp
bs_searcher searcher(10);

// Add some sets
binary_set bs1(10);
bs1.add(1); bs1.add(3);
searcher.add(101, bs1);

binary_set bs2(10);
bs2.add(1);
searcher.add(102, bs2);

// Find all subsets
binary_set query(10);
query.add(1); query.add(3); query.add(5);
auto results = searcher.find_subsets(query);  // Returns {101, 102}
```

## How to Build the Project

The project uses CMake for its build system.

**Prerequisites:**
*   A C++17 compliant compiler (e.g., GCC, Clang, MSVC).
*   CMake (version 3.10 or higher).
*   Git (to clone the repository and for `FetchContent` to download dependencies).

**Build Steps:**

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/zanzi/binary_set.git
    cd binary_set
    ```
2.  **Create a build directory and configure CMake:**
    ```bash
    cmake -B build
    ```
    This command tells CMake to generate build files (e.g., Makefiles on Unix-like systems, Visual Studio solutions on Windows) in a new directory named `build`.
3.  **Build the project:**
    ```bash
    cmake --build build
    ```
    This command compiles the source code and creates the executables (tests, benchmarks, examples) within the `build` directory.

## How to Run Tests

The project uses the Google Test framework for unit testing.

**Command to run tests:**
```bash
./build/tests/run_tests
```
This command will discover and execute all tests defined in [tests/binary_set_test.cpp](tests/binary_set_test.cpp) and [tests/bs_searcher_test.cpp](tests/bs_searcher_test.cpp).

## How to Run Benchmarks

Performance benchmarks are implemented using the Google Benchmark library.

**Command to run benchmarks:**
```bash
./build/benchmarks/run_benchmarks
```
This executable will run all defined benchmarks in [benchmarks/main_benchmark.cpp](benchmarks/main_benchmark.cpp), which compare the performance of `binary_set` against `std::set`, `std::unordered_set`, and `std::vector<bool>` for various operations like `add`, `remove`, `contains`, and set arithmetic.

For detailed performance benchmarks and memory usage analysis against standard library containers, please refer to [BENCHMARKS.md](BENCHMARKS.md).

## Installation

Simply copy [binary_set.hxx](binary_set.hxx) to your project directory and include it:

```cpp
#include "binary_set.hxx"
```

No compilation or linking required.

## Limitations

- Elements must be unsigned integers in range [0, capacity-1]
- Capacity must be known at construction time
- All sets in binary operations must have the same capacity

## Compiler Compatibility

Tested with:
- Latest GCC on Ubuntu (via GitHub Actions)
- Latest Clang on Ubuntu (via GitHub Actions)
- (Note: MSVC and older compiler versions are not explicitly tested by the current CI setup.)

## Contributing

Contributions are welcome! Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) file for detailed instructions on reporting bugs, suggesting features, and submitting pull requests.

## Development

See [CHANGELOG.md](CHANGELOG.md) for all notable changes.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## See Also

- [Examples](examples/) - More usage examples
- [Tests](tests/) - Unit tests

## Author

Matteo Zanella <matteozanella2@gmail.com>
