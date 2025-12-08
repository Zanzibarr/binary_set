# Usage Examples

This document provides comprehensive examples of using bs.hxx.

## Basic Usage

### Creating and Manipulating Sets

```cpp
#include "binary_set.hxx"
#include <iostream>

int main() {
    // Create an empty set with capacity 20
    binary_set bs(20);
    
    // Add elements
    bs.add(5);
    bs.add(10);
    bs.add(15);
    
    // Check if elements exist
    std::cout << "Contains 5? " << bs.contains(5) << "\n";      // true
    std::cout << "Contains 7? " << bs.contains(7) << "\n";      // false
    std::cout << "Contains 10? " << bs[10] << "\n";             // true (using operator[])
    
    // Remove elements
    if (bs.remove(10)) {
        std::cout << "Removed 10\n";
    }
    
    // Check set properties
    std::cout << "Size: " << bs.size() << "\n";                 // 2
    std::cout << "Capacity: " << bs.capacity() << "\n";         // 20
    std::cout << "Empty? " << bs.empty() << "\n";               // false
    
    return 0;
}
```

### Iterating Over Elements

```cpp
#include "binary_set.hxx"
#include <iostream>

int main() {
    binary_set bs(10);
    bs.add(2);
    bs.add(5);
    bs.add(7);
    
    // Range-based for loop
    std::cout << "Elements: ";
    for (unsigned int elem : bs) {
        std::cout << elem << " ";  // Output: 2 5 7
    }
    std::cout << "\n";
    
    // Manual iteration
    for (auto it = bs.begin(); it != bs.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    // Convert to vector
    auto vec = bs.sparse();
    for (unsigned int elem : vec) {
        std::cout << elem << " ";
    }
    
    return 0;
}
```

### Visualizing Sets

```cpp
#include "binary_set.hxx"
#include <iostream>
#include <string>

int main() {
    binary_set bs(16);
    bs.add(0);
    bs.add(3);
    bs.add(7);
    bs.add(15);
    
    // Convert to string representation
    std::string repr = static_cast<std::string>(bs);
    std::cout << repr << "\n";  // Output: [X--X---X-------X]
    // X = element present, - = element absent
    
    return 0;
}
```

## Set Operations

### Union, Intersection, and Difference

```cpp
#include "binary_set.hxx"
#include <iostream>

int main() {
    binary_set a(10);
    a.add(1); a.add(3); a.add(5); a.add(7);
    
    binary_set b(10);
    b.add(3); b.add(5); b.add(8); b.add(9);
    
    // Union: elements in either set
    auto union_set = a | b;  // {1, 3, 5, 7, 8, 9}
    
    // Intersection: elements in both sets
    auto intersection = a & b;  // {3, 5}
    
    // Difference: elements in a but not in b
    auto difference = a - b;  // {1, 7}
    
    // Complement: all elements not in a
    auto complement = !a;  // {0, 2, 4, 6, 8, 9}
    
    // In-place operations
    a |= b;  // a now contains union
    
    return 0;
}
```

### Set Comparisons

```cpp
#include "binary_set.hxx"
#include <iostream>

int main() {
    binary_set a(10);
    a.add(1); a.add(3); a.add(5);
    
    binary_set b(10);
    b.add(1); b.add(3); b.add(5);
    
    binary_set c(10);
    c.add(1); c.add(3);
    
    binary_set d(10);
    d.add(2); d.add(4);
    
    // Equality
    std::cout << "a == b? " << (a == b) << "\n";  // true
    std::cout << "a != c? " << (a != c) << "\n";  // true
    
    // Subset checking
    std::cout << "a contains c? " << a.contains(c) << "\n";  // true
    std::cout << "c contains a? " << c.contains(a) << "\n";  // false
    
    // Intersection checking
    std::cout << "a intersects c? " << a.intersects(c) << "\n";  // true
    std::cout << "a intersects d? " << a.intersects(d) << "\n";  // false
    
    return 0;
}
```

## Advanced: Subset Searching

### Basic Subset Search

```cpp
#include "binary_set.hxx"
#include <iostream>
#include <vector>

int main() {
    // Create a searcher for sets of capacity 8
    bs_searcher searcher(8);
    
    // Add some sets with IDs
    binary_set bs1(8);
    bs1.add(1); bs1.add(3);
    searcher.add(101, bs1);  // ID: 101
    
    binary_set bs2(8);
    bs2.add(1);
    searcher.add(102, bs2);  // ID: 102
    
    binary_set bs3(8);
    bs3.add(1); bs3.add(3); bs3.add(5);
    searcher.add(103, bs3);  // ID: 103
    
    // Query: find all subsets
    binary_set query(8);
    query.add(1); query.add(3); query.add(4); query.add(6);
    
    std::vector<unsigned int> results = searcher.find_subsets(query);
    // Results: {101, 102} because both are subsets of query
    // 103 is NOT included because it contains 5, which is not in query
    
    std::cout << "Found " << results.size() << " subsets:\n";
    for (unsigned int id : results) {
        std::cout << "  ID: " << id << "\n";
    }
    
    return 0;
}
```

### Product Recommendation System

```cpp
#include "binary_set.hxx"
#include <iostream>
#include <map>
#include <string>

int main() {
    // Each bit represents a feature
    // 0: Wireless, 1: Waterproof, 2: Budget, 3: Premium, 4: Compact
    
    const unsigned int NUM_FEATURES = 5;
    bs_searcher product_db(NUM_FEATURES);
    
    // Product catalog
    std::map<unsigned int, std::string> products = {
        {1, "Budget Wireless Earbuds"},
        {2, "Premium Wireless Waterproof Earbuds"},
        {3, "Compact Budget Earbuds"},
        {4, "Premium Waterproof Speaker"}
    };
    
    // Add products with their features
    binary_set prod1(NUM_FEATURES);
    prod1.add(0); prod1.add(2);  // Wireless, Budget
    product_db.add(1, prod1);
    
    binary_set prod2(NUM_FEATURES);
    prod2.add(0); prod2.add(1); prod2.add(3);  // Wireless, Waterproof, Premium
    product_db.add(2, prod2);
    
    binary_set prod3(NUM_FEATURES);
    prod3.add(2); prod3.add(4);  // Budget, Compact
    product_db.add(3, prod3);
    
    binary_set prod4(NUM_FEATURES);
    prod4.add(1); prod4.add(3);  // Waterproof, Premium
    product_db.add(4, prod4);
    
    // Customer wants: Wireless, Waterproof, Premium
    binary_set customer_needs(NUM_FEATURES);
    customer_needs.add(0);  // Wireless
    customer_needs.add(1);  // Waterproof
    customer_needs.add(3);  // Premium
    
    // Find matching products (those whose features are subset of needs)
    auto matches = product_db.find_subsets(customer_needs);
    
    std::cout << "Recommended products:\n";
    for (unsigned int id : matches) {
        std::cout << "  - " << products[id] << "\n";
    }
    // Output: Premium Wireless Waterproof Earbuds, Premium Waterproof Speaker
    
    return 0;
}
```

## Practical Applications

### Sudoku Solver - Tracking Possible Values

```cpp
#include "binary_set.hxx"
#include <array>

class SudokuCell {
    binary_set possible_values;
    
public:
    SudokuCell() : possible_values(9, true) {}  // Start with all values possible (1-9)
    
    void eliminate(unsigned int value) {
        if (value >= 1 && value <= 9) {
            possible_values.remove(value - 1);  // 0-indexed
        }
    }
    
    bool is_solved() const {
        return possible_values.size() == 1;
    }
    
    unsigned int get_value() const {
        if (is_solved()) {
            return *possible_values.begin() + 1;  // Convert back to 1-9
        }
        return 0;  // Unsolved
    }
    
    unsigned int num_possibilities() const {
        return possible_values.size();
    }
};
```

### Permission System

```cpp
#include "binary_set.hxx"
#include <string>
#include <map>

enum Permission {
    READ = 0,
    WRITE = 1,
    EXECUTE = 2,
    DELETE = 3,
    ADMIN = 4
};

class User {
    std::string name;
    binary_set permissions;
    
public:
    User(const std::string& name) : name(name), permissions(5) {}
    
    void grant(Permission perm) {
        permissions.add(static_cast<unsigned int>(perm));
    }
    
    void revoke(Permission perm) {
        permissions.remove(static_cast<unsigned int>(perm));
    }
    
    bool has_permission(Permission perm) const {
        return permissions[static_cast<unsigned int>(perm)];
    }
    
    bool has_all(const binary_set& required) const {
        return permissions.contains(required);
    }
};
```

### Graph Algorithms - Visited Nodes

```cpp
#include "binary_set.hxx"
#include <vector>
#include <queue>

void bfs(const std::vector<std::vector<unsigned int>>& graph, unsigned int start) {
    unsigned int n = graph.size();
    binary_set visited(n);
    std::queue<unsigned int> q;
    
    q.push(start);
    visited.add(start);
    
    while (!q.empty()) {
        unsigned int node = q.front();
        q.pop();
        
        // Process node...
        
        for (unsigned int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited.add(neighbor);
                q.push(neighbor);
            }
        }
    }
}
```

## Error Handling

```cpp
#include "binary_set.hxx"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        // Attempting to access element out of range
        binary_set bs(10);
        bs.add(15);  // Throws std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    try {
        // Operating on sets with different capacities
        binary_set a(10);
        binary_set b(20);
        auto c = a | b;  // Throws std::invalid_argument
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    try {
        // Creating set with capacity 0
        binary_set bs(0);  // Throws std::invalid_argument
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    return 0;
}
```

## Performance Tips

### Prefer Fill/Clear Over Individual Operations

```cpp
// Slow: Adding many elements individually
binary_set bs(1000);
for (unsigned int i = 0; i < 1000; i++) {
    bs.add(i);
}

// Fast: Fill all at once
binary_set bs2(1000, true);  // or bs2.fill()
```

### Use In-Place Operations

```cpp
binary_set a(100);
binary_set b(100);

// Creates temporary
a = a | b;

// More efficient
a |= b;
```

### Avoid Repeated Iteration for Sparse Sets

```cpp
binary_set bs(10000);
bs.add(5);
bs.add(9999);

// Slow: Iterates through all 10000 positions
for (unsigned int elem : bs) {
    process(elem);
}

// Fast: Convert to vector once
auto vec = bs.sparse();  // Only {5, 9999}
for (unsigned int elem : vec) {
    process(elem);
}
```