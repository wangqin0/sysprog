#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    int check_power_of_2(uint32_t n);
}

class PowerOf2CheckTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PowerOf2CheckTest, PowersOfTwo) {
    EXPECT_EQ(1, check_power_of_2(1));
    EXPECT_EQ(1, check_power_of_2(2));
    EXPECT_EQ(1, check_power_of_2(4));
    EXPECT_EQ(1, check_power_of_2(8));
    EXPECT_EQ(1, check_power_of_2(16));
    EXPECT_EQ(1, check_power_of_2(32));
    EXPECT_EQ(1, check_power_of_2(64));
    EXPECT_EQ(1, check_power_of_2(128));
    EXPECT_EQ(1, check_power_of_2(256));
    EXPECT_EQ(1, check_power_of_2(512));
    EXPECT_EQ(1, check_power_of_2(1024));
    EXPECT_EQ(1, check_power_of_2(2048));
    EXPECT_EQ(1, check_power_of_2(4096));
    EXPECT_EQ(1, check_power_of_2(8192));
    EXPECT_EQ(1, check_power_of_2(16384));
    EXPECT_EQ(1, check_power_of_2(32768));
    EXPECT_EQ(1, check_power_of_2(65536));
    EXPECT_EQ(1, check_power_of_2(131072));
    EXPECT_EQ(1, check_power_of_2(262144));
    EXPECT_EQ(1, check_power_of_2(524288));
    EXPECT_EQ(1, check_power_of_2(1048576));
    EXPECT_EQ(1, check_power_of_2(2097152));
    EXPECT_EQ(1, check_power_of_2(4194304));
    EXPECT_EQ(1, check_power_of_2(8388608));
    EXPECT_EQ(1, check_power_of_2(16777216));
    EXPECT_EQ(1, check_power_of_2(33554432));
    EXPECT_EQ(1, check_power_of_2(67108864));
    EXPECT_EQ(1, check_power_of_2(134217728));
    EXPECT_EQ(1, check_power_of_2(268435456));
    EXPECT_EQ(1, check_power_of_2(536870912));
    EXPECT_EQ(1, check_power_of_2(1073741824));
    EXPECT_EQ(1, check_power_of_2(2147483648U));
}

TEST_F(PowerOf2CheckTest, NonPowersOfTwo) {
    EXPECT_EQ(0, check_power_of_2(3));
    EXPECT_EQ(0, check_power_of_2(5));
    EXPECT_EQ(0, check_power_of_2(6));
    EXPECT_EQ(0, check_power_of_2(7));
    EXPECT_EQ(0, check_power_of_2(9));
    EXPECT_EQ(0, check_power_of_2(10));
    EXPECT_EQ(0, check_power_of_2(11));
    EXPECT_EQ(0, check_power_of_2(12));
    EXPECT_EQ(0, check_power_of_2(13));
    EXPECT_EQ(0, check_power_of_2(14));
    EXPECT_EQ(0, check_power_of_2(15));
    EXPECT_EQ(0, check_power_of_2(17));
    EXPECT_EQ(0, check_power_of_2(18));
    EXPECT_EQ(0, check_power_of_2(19));
    EXPECT_EQ(0, check_power_of_2(20));
    EXPECT_EQ(0, check_power_of_2(21));
    EXPECT_EQ(0, check_power_of_2(22));
    EXPECT_EQ(0, check_power_of_2(23));
    EXPECT_EQ(0, check_power_of_2(31));
    EXPECT_EQ(0, check_power_of_2(33));
    EXPECT_EQ(0, check_power_of_2(63));
    EXPECT_EQ(0, check_power_of_2(65));
    EXPECT_EQ(0, check_power_of_2(127));
    EXPECT_EQ(0, check_power_of_2(129));
    EXPECT_EQ(0, check_power_of_2(255));
    EXPECT_EQ(0, check_power_of_2(257));
    EXPECT_EQ(0, check_power_of_2(1023));
    EXPECT_EQ(0, check_power_of_2(1025));
}

TEST_F(PowerOf2CheckTest, EdgeCases) {
    EXPECT_EQ(1, check_power_of_2(0));  // Note: 0 passes the test (0-1)&0 = 0xFFFFFFFF&0 = 0
}

TEST_F(PowerOf2CheckTest, LargeNumbers) {
    EXPECT_EQ(0, check_power_of_2(4294967295U));  // 2^32 - 1
    EXPECT_EQ(0, check_power_of_2(4294967294U));  // 2^32 - 2
    EXPECT_EQ(0, check_power_of_2(3221225472U));  // 3 * 2^30
    EXPECT_EQ(0, check_power_of_2(2147483647U));  // 2^31 - 1
    EXPECT_EQ(0, check_power_of_2(2147483649U));  // 2^31 + 1
}