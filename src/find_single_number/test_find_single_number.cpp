#include <gtest/gtest.h>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <random>

extern "C" {
    int find_single_int(int *arr, int len);
}

class FindSingleNumberTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(FindSingleNumberTest, BasicSingleElement) {
    int arr[] = {42};
    EXPECT_EQ(42, find_single_int(arr, 1));
}

TEST_F(FindSingleNumberTest, ThreeElements) {
    int arr[] = {1, 2, 1};
    EXPECT_EQ(2, find_single_int(arr, 3));
}

TEST_F(FindSingleNumberTest, FiveElements) {
    int arr[] = {4, 1, 2, 1, 2};
    EXPECT_EQ(4, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, SingleElementAtStart) {
    int arr[] = {7, 3, 3, 5, 5};
    EXPECT_EQ(7, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, SingleElementAtEnd) {
    int arr[] = {3, 3, 5, 5, 7};
    EXPECT_EQ(7, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, SingleElementInMiddle) {
    int arr[] = {3, 3, 7, 5, 5};
    EXPECT_EQ(7, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, NegativeNumbers) {
    int arr[] = {-1, -2, -1, -3, -3};
    EXPECT_EQ(-2, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, MixedPositiveNegative) {
    int arr[] = {-5, 5, -5, 3, 3};
    EXPECT_EQ(5, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, Zero) {
    int arr[] = {0, 1, 1, 2, 2};
    EXPECT_EQ(0, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, ZeroAsSingle) {
    int arr[] = {1, 1, 0, 2, 2};
    EXPECT_EQ(0, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, LargePowerOfTwo) {
    int arr[] = {1024, 512, 512, 256, 256};
    EXPECT_EQ(1024, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, MaxInt) {
    int arr[] = {INT32_MAX, 100, 100, 50, 50};
    EXPECT_EQ(INT32_MAX, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, MinInt) {
    int arr[] = {INT32_MIN, 100, 100, 50, 50};
    EXPECT_EQ(INT32_MIN, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, LargeArray) {
    std::vector<int> arr;
    for (int i = 1; i <= 1000; i++) {
        arr.push_back(i);
        arr.push_back(i);
    }
    arr.push_back(1001);
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr.begin(), arr.end(), g);
    
    EXPECT_EQ(1001, find_single_int(arr.data(), arr.size()));
}

TEST_F(FindSingleNumberTest, DuplicatePattern) {
    int arr[] = {1, 1, 2, 2, 3, 3, 4, 4, 5};
    EXPECT_EQ(5, find_single_int(arr, 9));
}

TEST_F(FindSingleNumberTest, ManyDuplicates) {
    int arr[] = {7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13};
    EXPECT_EQ(13, find_single_int(arr, 13));
}

TEST_F(FindSingleNumberTest, AlternatingPattern) {
    int arr[] = {1, 2, 1, 3, 2};
    EXPECT_EQ(3, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, RepeatedSequence) {
    int arr[] = {5, 10, 15, 5, 10, 15, 20};
    EXPECT_EQ(20, find_single_int(arr, 7));
}

TEST_F(FindSingleNumberTest, BinaryValues) {
    int arr[] = {0, 0, 1, 1, 2, 2, 4, 4, 8};
    EXPECT_EQ(8, find_single_int(arr, 9));
}

TEST_F(FindSingleNumberTest, PowersOfTwoPlusOne) {
    int arr[] = {2, 2, 4, 4, 8, 8, 16, 16, 33};
    EXPECT_EQ(33, find_single_int(arr, 9));
}

TEST_F(FindSingleNumberTest, ConsecutiveNumbers) {
    int arr[] = {100, 100, 101, 101, 102, 102, 103, 103, 104};
    EXPECT_EQ(104, find_single_int(arr, 9));
}

TEST_F(FindSingleNumberTest, HexValues) {
    int arr[] = {0xFF, 0xFF, 0xAA, 0xAA, 0x55};
    EXPECT_EQ(0x55, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, PrimeNumbers) {
    int arr[] = {2, 2, 3, 3, 5, 5, 7, 7, 11, 11, 13, 13, 17};
    EXPECT_EQ(17, find_single_int(arr, 13));
}

TEST_F(FindSingleNumberTest, EdgeCaseSmallNegative) {
    int arr[] = {-1, -1, -2, -2, -3};
    EXPECT_EQ(-3, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, EdgeCaseLargeNegative) {
    int arr[] = {-1000, -1000, -2000, -2000, -3000};
    EXPECT_EQ(-3000, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, MixedSignsWithZero) {
    int arr[] = {-5, -5, 0, 0, 5};
    EXPECT_EQ(5, find_single_int(arr, 5));
}

TEST_F(FindSingleNumberTest, ReversedDuplicates) {
    int arr[] = {9, 8, 9, 7, 8, 6, 7};
    EXPECT_EQ(6, find_single_int(arr, 7));
}