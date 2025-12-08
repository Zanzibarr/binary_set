#include <iostream>
#include <vector>

#include "../binary_set.hxx"

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
