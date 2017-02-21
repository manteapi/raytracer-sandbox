#include <iostream>
#include <gtest/gtest.h>

TEST(Default, Signum)
{
    int a = 0;
    EXPECT_EQ(a, 0);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
