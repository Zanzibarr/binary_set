#include <iostream>
#include <string>

#include "../binary_set.hxx"

int main() {
    binary_set a(10);
    a.add(1);
    a.add(3);
    a.add(5);
    a.add(7);

    binary_set b(10);
    b.add(3);
    b.add(5);
    b.add(8);
    b.add(9);

    // Union: elements in either set
    auto union_set = a | b;  // {1, 3, 5, 7, 8, 9}
    std::cout << "Union: " << static_cast<std::string>(union_set) << std::endl;

    // Intersection: elements in both sets
    auto intersection = a & b;  // {3, 5}
    std::cout << "Intersection: " << static_cast<std::string>(intersection) << std::endl;

    // Difference: elements in a but not in b
    auto difference = a - b;  // {1, 7}
    std::cout << "Difference: " << static_cast<std::string>(difference) << std::endl;

    // Complement: all elements not in a
    auto complement = !a;  // {0, 2, 4, 6, 8, 9}
    std::cout << "Complement: " << static_cast<std::string>(complement) << std::endl;

    // In-place operations
    a |= b;  // a now contains union
    std::cout << "In-place union: " << static_cast<std::string>(a) << std::endl;

    return 0;
}
