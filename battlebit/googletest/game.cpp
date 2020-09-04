#include "gtest/gtest.h"
extern "C" {
#include "game.h"
}

TEST(xy_to_bitval, good_values) {
    // row 1
    EXPECT_EQ(xy_to_bitval(0, 0), 1ull);
    EXPECT_EQ(xy_to_bitval(1, 0), 2ull);
    EXPECT_EQ(xy_to_bitval(2, 0), 4ull);
    EXPECT_EQ(xy_to_bitval(3, 0), 8ull);
    EXPECT_EQ(xy_to_bitval(4, 0), 16ull);
    EXPECT_EQ(xy_to_bitval(5, 0), 32ull);
    EXPECT_EQ(xy_to_bitval(6, 0), 64ull);
    EXPECT_EQ(xy_to_bitval(7, 0), 128ull);

    // row 2
    EXPECT_EQ(xy_to_bitval(0, 1), 256ull);
    EXPECT_EQ(xy_to_bitval(1, 1), 512ull);
    EXPECT_EQ(xy_to_bitval(2, 1), 1024ull);
    EXPECT_EQ(xy_to_bitval(3, 1), 2048ull);
    EXPECT_EQ(xy_to_bitval(4, 1), 4096ull);
    EXPECT_EQ(xy_to_bitval(5, 1), 8192ull);
    EXPECT_EQ(xy_to_bitval(6, 1), 16384ull);
    EXPECT_EQ(xy_to_bitval(7, 1), 32768ull);

    // start of row 7
    EXPECT_EQ(xy_to_bitval(0, 7), 1ull << 56ull);

    // end of row 7
    EXPECT_EQ(xy_to_bitval(7, 7), 1ull << 63ull);
}

TEST(xy_to_bitval, bad_values) {
    EXPECT_EQ(xy_to_bitval(0, 8), 0);
    EXPECT_EQ(xy_to_bitval(8, 0), 0);
    EXPECT_EQ(xy_to_bitval(-1, 0), 0);
    EXPECT_EQ(xy_to_bitval(0, -1), 0);
}