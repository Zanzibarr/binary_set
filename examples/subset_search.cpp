#include <iostream>
#include <vector>

#include "../binary_set.hxx"

int main() {
    // Create a searcher for sets of capacity 8
    bs_searcher searcher(8);

    // Add some sets with IDs
    binary_set bs1(8);
    bs1.add(1);
    bs1.add(3);
    searcher.add(101, bs1);  // ID: 101

    binary_set bs2(8);
    bs2.add(1);
    searcher.add(102, bs2);  // ID: 102

    binary_set bs3(8);
    bs3.add(1);
    bs3.add(3);
    bs3.add(5);
    searcher.add(103, bs3);  // ID: 103

    // Query: find all subsets
    binary_set query(8);
    query.add(1);
    query.add(3);
    query.add(4);
    query.add(6);

    std::vector<unsigned int> results = searcher.find_subsets(query);
    // Results: {101, 102} because both are subsets of query
    // 103 is NOT included because it contains 5, which is not in query

    std::cout << "Found " << results.size() << " subsets:\n";
    for (unsigned int id : results) {
        std::cout << "  ID: " << id << "\n";
    }

    return 0;
}
