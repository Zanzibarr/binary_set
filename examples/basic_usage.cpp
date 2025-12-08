#include <iostream>

#include "../binary_set.hxx"

int main() {
    // Create an empty set with capacity 20
    binary_set bs(20);

    // Add elements
    bs.add(5);
    bs.add(10);
    bs.add(15);

    // Check if elements exist
    std::cout << "Contains 5? " << bs.contains(5) << "\n";  // true
    std::cout << "Contains 7? " << bs.contains(7) << "\n";  // false
    std::cout << "Contains 10? " << bs[10] << "\n";         // true (using operator[])

    // Remove elements
    if (bs.remove(10)) {
        std::cout << "Removed 10\n";
    }

    // Check set properties
    std::cout << "Size: " << bs.size() << "\n";          // 2
    std::cout << "Capacity: " << bs.capacity() << "\n";  // 20
    std::cout << "Empty? " << bs.empty() << "\n";        // false

    return 0;
}
