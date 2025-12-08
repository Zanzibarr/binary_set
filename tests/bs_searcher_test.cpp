#include "../binary_set.hxx"
#include "gtest/gtest.h"

TEST(BSSearcherTest, Constructor) {
    bs_searcher searcher(10);
    binary_set bs(10);
    // Should not throw
    searcher.find_subsets(bs);
}

TEST(BSSearcherTest, AddAndFind) {
    bs_searcher searcher(8);

    binary_set bs1(8);
    bs1.add(1);
    bs1.add(3);
    searcher.add(101, bs1);

    binary_set bs2(8);
    bs2.add(1);
    searcher.add(102, bs2);

    binary_set bs3(8);
    bs3.add(1);
    bs3.add(3);
    bs3.add(5);
    searcher.add(103, bs3);

    binary_set query(8);
    query.add(1);
    query.add(3);
    query.add(4);
    query.add(6);

    std::vector<unsigned int> results = searcher.find_subsets(query);
    std::sort(results.begin(), results.end());

    std::vector<unsigned int> expected = {101, 102};
    EXPECT_EQ(results, expected);
}

TEST(BSSearcherTest, AddMixedPaths) {
    bs_searcher searcher(2);  // Capacity 2 for simple paths

    binary_set bs_00(2);  // Element 0: false, Element 1: false
    searcher.add(1, bs_00);

    binary_set bs_01(2);  // Element 0: false, Element 1: true
    bs_01.add(1);
    searcher.add(2, bs_01);

    binary_set bs_10(2);  // Element 0: true, Element 1: false
    bs_10.add(0);
    searcher.add(3, bs_10);

    binary_set bs_11(2);  // Element 0: true, Element 1: true
    bs_11.add(0);
    bs_11.add(1);
    searcher.add(4, bs_11);

    // Verify all can be found
    binary_set query_all(2, true);
    std::vector<unsigned int> results = searcher.find_subsets(query_all);
    std::sort(results.begin(), results.end());
    std::vector<unsigned int> expected = {1, 2, 3, 4};
    EXPECT_EQ(results, expected);
}

TEST(BSSearcherTest, AddWithInvalidCapacity) {
    bs_searcher searcher(8);
    binary_set bs(10);
    EXPECT_THROW(searcher.add(101, bs), std::invalid_argument);
}

TEST(BSSearcherTest, FindWithInvalidCapacity) {
    bs_searcher searcher(8);
    binary_set bs(10);
    EXPECT_THROW(searcher.find_subsets(bs), std::invalid_argument);
}

TEST(BSSearcherTest, Remove) {
    bs_searcher searcher(8);

    binary_set bs1(8);
    bs1.add(1);
    bs1.add(3);
    searcher.add(101, bs1);

    binary_set bs2(8);
    bs2.add(1);
    searcher.add(102, bs2);

    EXPECT_TRUE(searcher.remove(101, bs1));

    binary_set query(8);
    query.add(1);
    query.add(3);
    query.add(4);

    std::vector<unsigned int> results = searcher.find_subsets(query);
    std::vector<unsigned int> expected = {102};
    EXPECT_EQ(results, expected);

    EXPECT_FALSE(searcher.remove(101, bs1));
}

TEST(BSSearcherTest, RemoveWithInvalidCapacity) {
    bs_searcher searcher(8);
    binary_set bs(10);
    EXPECT_THROW(searcher.remove(101, bs), std::invalid_argument);
}

TEST(BSSearcherTest, FindNoSubsets) {
    bs_searcher searcher(8);

    binary_set bs1(8);
    bs1.add(1);
    bs1.add(3);
    searcher.add(101, bs1);

    binary_set query(8);
    query.add(2);
    query.add(4);

    std::vector<unsigned int> results = searcher.find_subsets(query);
    EXPECT_TRUE(results.empty());
}

TEST(BSSearcherTest, FindMixedPaths) {
    bs_searcher searcher(2);

    binary_set bs_01(2);  // 01
    bs_01.add(1);
    searcher.add(1, bs_01);

    binary_set bs_10(2);  // 10
    bs_10.add(0);
    searcher.add(2, bs_10);

    binary_set bs_11(2);  // 11
    bs_11.add(0);
    bs_11.add(1);
    searcher.add(3, bs_11);

    // Query 1X (element 0 is true, element 1 can be anything)
    binary_set query_1X(2);
    query_1X.add(0);  // Element 0 is true, 1 is false (implies X)
    std::vector<unsigned int> results_1X = searcher.find_subsets(query_1X);
    std::sort(results_1X.begin(), results_1X.end());
    std::vector<unsigned int> expected_1X = {2};  // Only bs_10
    EXPECT_EQ(results_1X, expected_1X);

    // Query X1 (element 1 is true, element 0 can be anything)
    binary_set query_X1(2);
    query_X1.add(1);  // Element 1 is true, 0 is false (implies X)
    std::vector<unsigned int> results_X1 = searcher.find_subsets(query_X1);
    std::sort(results_X1.begin(), results_X1.end());
    std::vector<unsigned int> expected_X1 = {1};  // Only bs_01
    EXPECT_EQ(results_X1, expected_X1);

    // Query 0X (element 0 is false, element 1 can be anything)
    binary_set query_0X(2);
    // Element 0 is false, 1 is false (implies X)
    std::vector<unsigned int> results_0X = searcher.find_subsets(query_0X);
    std::sort(results_0X.begin(), results_0X.end());
    std::vector<unsigned int> expected_0X = {};
    EXPECT_EQ(results_0X, expected_0X);
}

TEST(BSSearcherTest, FindAllSubsets) {
    bs_searcher searcher(8);

    binary_set bs1(8);
    bs1.add(1);
    searcher.add(101, bs1);

    binary_set bs2(8);
    bs2.add(1);
    bs2.add(3);
    searcher.add(102, bs2);

    binary_set query(8, true);  // a full set

    std::vector<unsigned int> results = searcher.find_subsets(query);
    std::sort(results.begin(), results.end());
    std::vector<unsigned int> expected = {101, 102};
    EXPECT_EQ(results, expected);
}

TEST(BSSearcherTest, RemoveAndPrune) {
    bs_searcher searcher(4);

    binary_set bs1(4);
    bs1.add(1);
    searcher.add(1, bs1);

    binary_set bs2(4);
    bs2.add(1);
    bs2.add(2);
    searcher.add(2, bs2);

    EXPECT_TRUE(searcher.remove(1, bs1));

    binary_set query(4, true);
    auto result = searcher.find_subsets(query);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 2);

    EXPECT_TRUE(searcher.remove(2, bs2));

    result = searcher.find_subsets(query);
    ASSERT_TRUE(result.empty());
}

TEST(BSSearcherTest, RemoveNonLastValueAndPrune) {
    bs_searcher searcher(2);
    binary_set bs_10(2);
    bs_10.add(0);
    searcher.add(1, bs_10);
    searcher.add(2, bs_10);  // Duplicate value

    binary_set bs_01(2);
    bs_01.add(1);
    searcher.add(3, bs_01);  // Another path

    // Remove the first value (1) from bs_10 path, which is not the last
    // Values: [1, 2] -> remove 1 -> [2]
    EXPECT_TRUE(searcher.remove(1, bs_10));

    binary_set query_10(2);
    query_10.add(0);
    std::vector<unsigned int> results = searcher.find_subsets(query_10);
    std::sort(results.begin(), results.end());
    std::vector<unsigned int> expected = {2};  // Only 2 should remain for bs_10 path
    EXPECT_EQ(results, expected);

    // Remove the last value (2), now the node becomes empty and should prune
    EXPECT_TRUE(searcher.remove(2, bs_10));
    results = searcher.find_subsets(query_10);
    EXPECT_TRUE(results.empty());  // Path should be pruned

    // Ensure other paths are unaffected
    binary_set query_01(2);
    query_01.add(1);
    results = searcher.find_subsets(query_01);
    std::vector<unsigned int> expected_01 = {3};
    EXPECT_EQ(results, expected_01);
}

TEST(BSSearcherTest, RemoveWithPartialPruning) {
    bs_searcher searcher(1);  // Simpler capacity
    binary_set bs_0(1);       // Element 0: false
    searcher.add(1, bs_0);
    searcher.add(2, bs_0);  // Add a duplicate to ensure values vector is not empty immediately

    binary_set bs_1(1);  // Element 0: true
    bs_1.add(0);
    searcher.add(3, bs_1);

    // Remove one instance of bs_0. The node's values vector still has '2'. So, no pruning yet.
    EXPECT_TRUE(searcher.remove(1, bs_0));

    binary_set query_0(1);  // Query for sets with element 0 absent
    std::vector<unsigned int> results_0 = searcher.find_subsets(query_0);
    std::sort(results_0.begin(), results_0.end());
    std::vector<unsigned int> expected_0 = {2};
    EXPECT_EQ(results_0, expected_0);

    // Remove the last instance of bs_0. Now node->values is empty.
    // The node itself has no children (capacity 1), so it should prune.
    EXPECT_TRUE(searcher.remove(2, bs_0));
    results_0 = searcher.find_subsets(query_0);
    EXPECT_TRUE(results_0.empty());  // Path should be pruned up to the root or parent with other child/value

    // The other path (bs_1) should still exist
    binary_set query_1(1);
    query_1.add(0);  // Query for sets with element 0 present
    std::vector<unsigned int> results_1 = searcher.find_subsets(query_1);
    std::sort(results_1.begin(), results_1.end());
    std::vector<unsigned int> expected_1 = {3};
    EXPECT_EQ(results_1, expected_1);
}

TEST(BSSearcherTest, RemoveAndPruneComplex) {
    bs_searcher searcher(5);
    binary_set bs1(5);
    bs1.add(1);
    searcher.add(1, bs1);

    binary_set bs2(5);
    bs2.add(1);
    bs2.add(3);
    searcher.add(2, bs2);

    binary_set bs3(5);
    bs3.add(1);
    bs3.add(4);
    searcher.add(3, bs3);

    binary_set query(5, true);
    auto result = searcher.find_subsets(query);
    EXPECT_EQ(result.size(), 3);

    // Remove {1, 3}, path to {1} and {1, 4} should remain
    EXPECT_TRUE(searcher.remove(2, bs2));
    result = searcher.find_subsets(query);
    EXPECT_EQ(result.size(), 2);

    // Remove {1, 4}, branch from {1} should be pruned
    EXPECT_TRUE(searcher.remove(3, bs3));
    result = searcher.find_subsets(query);
    EXPECT_EQ(result.size(), 1);

    // Remove {1}, whole branch should be gone
    EXPECT_TRUE(searcher.remove(1, bs1));
    result = searcher.find_subsets(query);
    EXPECT_TRUE(result.empty());
}

TEST(BSSearcherTest, Duplicates) {
    bs_searcher searcher(4);
    binary_set bs(4);
    bs.add(1);

    // Add the same set with the same ID twice
    searcher.add(1, bs);
    searcher.add(1, bs);

    auto result = searcher.find_subsets(bs);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 1);

    // Remove the set once, one ID should remain
    EXPECT_TRUE(searcher.remove(1, bs));
    result = searcher.find_subsets(bs);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 1);

    // Remove the set again, it should now be gone
    EXPECT_TRUE(searcher.remove(1, bs));
    result = searcher.find_subsets(bs);
    ASSERT_TRUE(result.empty());
}

TEST(BSSearcherTest, RemoveNonExistent) {
    bs_searcher searcher(4);
    binary_set bs(4);
    bs.add(1);

    binary_set bs2(4);
    bs2.add(2);

    searcher.add(1, bs);

    EXPECT_FALSE(searcher.remove(2, bs));
    EXPECT_FALSE(searcher.remove(1, bs2));
}
