#include "../binary_set.hxx"

#include "gtest/gtest.h"

TEST(BinarySetTest, DefaultConstructor) {
    binary_set bs;
    EXPECT_EQ(bs.capacity(), 0);
    EXPECT_EQ(bs.size(), 0);
    EXPECT_TRUE(bs.empty());
}

TEST(BinarySetTest, ConstructorWithCapacity) {
    binary_set bs(10);
    EXPECT_EQ(bs.capacity(), 10);
    EXPECT_EQ(bs.size(), 0);
    EXPECT_TRUE(bs.empty());

    binary_set bs_filled(10, true);
    EXPECT_EQ(bs_filled.capacity(), 10);
    EXPECT_EQ(bs_filled.size(), 10);
    EXPECT_FALSE(bs_filled.empty());
}

TEST(BinarySetTest, ConstructorWithCapacityNotMultipleOf8) {
    binary_set bs(13);
    EXPECT_EQ(bs.capacity(), 13);
    EXPECT_EQ(bs.size(), 0);
    EXPECT_TRUE(bs.empty());

    binary_set bs_filled(13, true);
    EXPECT_EQ(bs_filled.capacity(), 13);
    EXPECT_EQ(bs_filled.size(), 13);
    EXPECT_FALSE(bs_filled.empty());
    for (unsigned int i = 0; i < 13; ++i) {
        EXPECT_TRUE(bs_filled.contains(i));
    }
}

TEST(BinarySetTest, InvalidConstructor) { EXPECT_THROW(binary_set(0), std::invalid_argument); }

TEST(BinarySetTest, Add) {
    binary_set bs(10);
    EXPECT_TRUE(bs.add(5));
    EXPECT_EQ(bs.size(), 1);
    EXPECT_TRUE(bs.contains(5));

    EXPECT_FALSE(bs.add(5));
    EXPECT_EQ(bs.size(), 1);

    EXPECT_THROW(bs.add(10), std::out_of_range);
    EXPECT_THROW(bs.add(15), std::out_of_range);
}

TEST(BinarySetTest, AddToZeroCapacitySet) {
    binary_set bs;
    EXPECT_THROW(bs.add(0), std::domain_error);
}

TEST(BinarySetTest, Remove) {
    binary_set bs(10);
    bs.add(5);
    EXPECT_TRUE(bs.remove(5));
    EXPECT_EQ(bs.size(), 0);
    EXPECT_FALSE(bs.contains(5));

    EXPECT_FALSE(bs.remove(5));
    EXPECT_EQ(bs.size(), 0);

    EXPECT_THROW(bs.remove(10), std::out_of_range);
}

TEST(BinarySetTest, RemoveFromZeroCapacitySet) {
    binary_set bs;
    EXPECT_THROW(bs.remove(0), std::domain_error);
}

TEST(BinarySetTest, Clear) {
    binary_set bs(10);
    bs.add(1);
    bs.add(5);
    bs.add(9);
    bs.clear();
    EXPECT_EQ(bs.size(), 0);
    EXPECT_TRUE(bs.empty());
    EXPECT_FALSE(bs.contains(1));
    EXPECT_FALSE(bs.contains(5));
    EXPECT_FALSE(bs.contains(9));
}

TEST(BinarySetTest, Fill) {
    binary_set bs(10);
    bs.fill();
    EXPECT_EQ(bs.size(), 10);
    EXPECT_FALSE(bs.empty());
    for (unsigned int i = 0; i < 10; ++i) {
        EXPECT_TRUE(bs.contains(i));
    }

    binary_set bs2(13);
    bs2.fill();
    EXPECT_EQ(bs2.size(), 13);
    for (unsigned int i = 0; i < 13; ++i) {
        EXPECT_TRUE(bs2.contains(i));
    }

    // New test case: capacity is a multiple of 8
    binary_set bs3(8);
    bs3.fill();
    EXPECT_EQ(bs3.size(), 8);
    for (unsigned int i = 0; i < 8; ++i) {
        EXPECT_TRUE(bs3.contains(i));
    }

    // New test case: default constructed binary_set (capacity 0)
    binary_set bs0;
    // Calling fill on a 0-capacity set should not change size and should remain empty
    bs0.fill();  // This call won't throw because validate_element isn't called
    EXPECT_EQ(bs0.size(), 0);
    EXPECT_TRUE(bs0.empty());
}

TEST(BinarySetTest, Contains) {
    binary_set bs(10);
    bs.add(5);
    EXPECT_TRUE(bs.contains(5));
    EXPECT_FALSE(bs.contains(0));
    EXPECT_THROW(bs.contains(10), std::out_of_range);
}

TEST(BinarySetTest, ContainsOnZeroCapacitySet) {
    const binary_set bs;
    EXPECT_THROW(bs.contains(0), std::domain_error);
}

TEST(BinarySetTest, SubscriptOperator) {
    binary_set bs(10);
    bs.add(5);
    EXPECT_TRUE(bs[5]);
    EXPECT_FALSE(bs[0]);
    EXPECT_THROW(bs[10], std::out_of_range);
}

TEST(BinarySetTest, Sparse) {
    binary_set bs(10);
    bs.add(1);
    bs.add(5);
    bs.add(9);
    std::vector<unsigned int> expected = {1, 5, 9};
    EXPECT_EQ(bs.sparse(), expected);

    binary_set empty_bs(10);
    EXPECT_TRUE(empty_bs.sparse().empty());
}

TEST(BinarySetTest, SparseOnZeroCapacitySet) {
    binary_set bs;
    EXPECT_THROW(bs.sparse(), std::domain_error);
}

TEST(BinarySetTest, StringOperator) {
    binary_set bs(10);
    bs.add(0);
    bs.add(3);
    bs.add(9);
    EXPECT_EQ(static_cast<std::string>(bs), "[X--X-----X]");
}

TEST(BinarySetTest, Intersection) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    a.add(5);
    binary_set b(10);
    b.add(3);
    b.add(5);
    b.add(7);

    binary_set c = a & b;
    EXPECT_EQ(c.size(), 2);
    EXPECT_TRUE(c.contains(3));
    EXPECT_TRUE(c.contains(5));

    binary_set d(11);
    EXPECT_THROW(a & d, std::invalid_argument);
}

TEST(BinarySetTest, IntersectionInPlace) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    a.add(5);
    binary_set b(10);
    b.add(3);
    b.add(5);
    b.add(7);

    a &= b;
    EXPECT_EQ(a.size(), 2);
    EXPECT_TRUE(a.contains(3));
    EXPECT_TRUE(a.contains(5));
}

TEST(BinarySetTest, Union) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    binary_set b(10);
    b.add(3);
    b.add(5);

    binary_set c = a | b;
    EXPECT_EQ(c.size(), 3);
    EXPECT_TRUE(c.contains(1));
    EXPECT_TRUE(c.contains(3));
    EXPECT_TRUE(c.contains(5));

    binary_set d(11);
    EXPECT_THROW(a | d, std::invalid_argument);
}

TEST(BinarySetTest, UnionInPlace) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    binary_set b(10);
    b.add(3);
    b.add(5);

    a |= b;
    EXPECT_EQ(a.size(), 3);
    EXPECT_TRUE(a.contains(1));
    EXPECT_TRUE(a.contains(3));
    EXPECT_TRUE(a.contains(5));
}

TEST(BinarySetTest, Difference) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    a.add(5);
    binary_set b(10);
    b.add(3);
    b.add(5);
    b.add(7);

    binary_set c = a - b;
    EXPECT_EQ(c.size(), 1);
    EXPECT_TRUE(c.contains(1));

    binary_set d(11);
    EXPECT_THROW(a - d, std::invalid_argument);
}

TEST(BinarySetTest, DifferenceInPlace) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    a.add(5);
    binary_set b(10);
    b.add(3);
    b.add(5);
    b.add(7);

    a -= b;
    EXPECT_EQ(a.size(), 1);
    EXPECT_TRUE(a.contains(1));
}

TEST(BinarySetTest, Complement) {
    binary_set bs(10);
    bs.add(1);
    bs.add(3);
    bs.add(5);
    binary_set c = !bs;
    EXPECT_EQ(c.size(), 7);
    EXPECT_TRUE(c.contains(0));
    EXPECT_TRUE(c.contains(2));
    EXPECT_TRUE(c.contains(4));
    EXPECT_TRUE(c.contains(6));
    EXPECT_TRUE(c.contains(7));
    EXPECT_TRUE(c.contains(8));
    EXPECT_TRUE(c.contains(9));
}

TEST(BinarySetTest, ComplementMultipleOf8) {
    binary_set bs(8);  // Capacity is a multiple of 8
    bs.add(1);
    bs.add(2);
    binary_set c = !bs;
    EXPECT_EQ(c.size(), 6);
    EXPECT_TRUE(c.contains(0));
    EXPECT_TRUE(c.contains(3));
    EXPECT_TRUE(c.contains(4));
    EXPECT_TRUE(c.contains(5));
    EXPECT_TRUE(c.contains(6));
    EXPECT_TRUE(c.contains(7));
}

TEST(BinarySetTest, ComplementNotMultipleOf8) {
    binary_set bs(13);
    bs.add(1);
    bs.add(3);
    bs.add(5);
    binary_set c = !bs;
    EXPECT_EQ(c.size(), 10);
    EXPECT_TRUE(c.contains(0));
    EXPECT_TRUE(c.contains(2));
    EXPECT_TRUE(c.contains(4));
    for (int i = 6; i < 13; ++i) {
        EXPECT_TRUE(c.contains(i));
    }
}

TEST(BinarySetTest, Equality) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    binary_set b(10);
    b.add(1);
    b.add(3);
    binary_set c(10);
    c.add(1);
    c.add(4);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);

    binary_set d(11);
    EXPECT_THROW(a == d, std::invalid_argument);
}

TEST(BinarySetTest, Inequality) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    binary_set b(10);
    b.add(1);
    b.add(3);
    binary_set c(10);
    c.add(1);
    c.add(4);

    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);

    binary_set d(11);
    EXPECT_THROW(a != d, std::invalid_argument);
}

TEST(BinarySetTest, Intersects) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    binary_set b(10);
    b.add(3);
    b.add(5);
    binary_set c(10);
    c.add(2);
    c.add(4);

    EXPECT_TRUE(a.intersects(b));
    EXPECT_FALSE(a.intersects(c));

    binary_set d(11);
    EXPECT_THROW(a.intersects(d), std::invalid_argument);
}

TEST(BinarySetTest, ContainsSet) {
    binary_set a(10);
    a.add(1);
    a.add(3);
    a.add(5);
    binary_set b(10);
    b.add(1);
    b.add(3);
    binary_set c(10);
    c.add(1);
    c.add(4);

    EXPECT_TRUE(a.contains(b));
    EXPECT_FALSE(a.contains(c));
    EXPECT_FALSE(b.contains(a));

    binary_set d(11);
    EXPECT_THROW(a.contains(d), std::invalid_argument);
}

TEST(BinarySetTest, Iterator) {
    binary_set bs(20);
    bs.add(5);
    bs.add(10);
    bs.add(15);

    std::vector<unsigned int> expected = {5, 10, 15};
    std::vector<unsigned int> actual;
    for (unsigned int elem : bs) {
        actual.push_back(elem);
    }
    EXPECT_EQ(actual, expected);
}

TEST(BinarySetTest, IteratorEmptySet) {
    binary_set bs(20);
    std::vector<unsigned int> actual;
    for (unsigned int elem : bs) {
        actual.push_back(elem);
    }
    EXPECT_TRUE(actual.empty());
}

TEST(BinarySetTest, IteratorFullSet) {
    binary_set bs(10, true);
    std::vector<unsigned int> expected;
    for (unsigned int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    std::vector<unsigned int> actual;
    for (unsigned int elem : bs) {
        actual.push_back(elem);
    }
    EXPECT_EQ(actual, expected);
}

TEST(BinarySetTest, IteratorPostIncrement) {
    binary_set bs(10);
    bs.add(1);
    bs.add(5);
    auto it = bs.begin();
    EXPECT_EQ(*(it++), 1);
    EXPECT_EQ(*it, 5);
}

TEST(BinarySetTest, CopyConstructor) {
    binary_set a(10);
    a.add(1);
    a.add(5);
    binary_set b = a;
    EXPECT_EQ(a.size(), b.size());
    EXPECT_EQ(a.capacity(), b.capacity());
    EXPECT_TRUE(b.contains(1));
    EXPECT_TRUE(b.contains(5));
}

TEST(BinarySetTest, CopyAssignment) {
    binary_set a(10);
    a.add(1);
    a.add(5);
    binary_set b(5);
    b = a;
    EXPECT_EQ(a.size(), b.size());
    EXPECT_EQ(a.capacity(), b.capacity());
    EXPECT_TRUE(b.contains(1));
    EXPECT_TRUE(b.contains(5));
}
