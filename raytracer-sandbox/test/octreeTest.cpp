#include <iostream>
#include <gtest/gtest.h>
#include <raytracer-sandbox/octree.hpp>

using namespace std;

TEST(Octree, Constructor_Normal)
{
    EXPECT_EQ(true, true);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
