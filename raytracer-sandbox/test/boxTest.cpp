#include <iostream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/box.hpp>

TEST(Box, Constructor)
{
    glm::vec3 minBB(0,0,0), maxBB(1,1,1);
    Box box(minBB, maxBB);
    std::array<glm::vec3,2> bounds = box.bounds();
    EXPECT_EQ(bounds[0][0], 0);
    EXPECT_EQ(bounds[0][1], 0);
    EXPECT_EQ(bounds[0][2], 0);
    EXPECT_EQ(bounds[1][0], 1);
    EXPECT_EQ(bounds[1][1], 1);
    EXPECT_EQ(bounds[1][2], 1);
}

TEST(Box, GetterSetter)
{
    //Getter
    glm::vec3 minBB(0,0,0), maxBB(1,1,1);
    const Box constBox(minBB, maxBB);
    std::array<glm::vec3,2> bounds = constBox.bounds();
    EXPECT_EQ(bounds[0][0], 0);
    EXPECT_EQ(bounds[0][1], 0);
    EXPECT_EQ(bounds[0][2], 0);
    EXPECT_EQ(bounds[1][0], 1);
    EXPECT_EQ(bounds[1][1], 1);
    EXPECT_EQ(bounds[1][2], 1);

    glm::vec3 minExtent = constBox.minExtent();
    EXPECT_EQ(minExtent[0], 0);
    EXPECT_EQ(minExtent[1], 0);
    EXPECT_EQ(minExtent[2], 0);

    glm::vec3 maxExtent = constBox.maxExtent();
    EXPECT_EQ(maxExtent[0], 1);
    EXPECT_EQ(maxExtent[1], 1);
    EXPECT_EQ(maxExtent[2], 1);

    //Setter
    Box box;
    box.bounds() = bounds;
    EXPECT_EQ(box.bounds()[0][0], 0);
    EXPECT_EQ(box.bounds()[0][1], 0);
    EXPECT_EQ(box.bounds()[0][2], 0);
    EXPECT_EQ(box.bounds()[1][0], 1);
    EXPECT_EQ(box.bounds()[1][1], 1);
    EXPECT_EQ(box.bounds()[1][2], 1);

    box.minExtent() = glm::vec3(1,1,1);
    EXPECT_EQ(box.minExtent()[0], 1);
    EXPECT_EQ(box.minExtent()[1], 1);
    EXPECT_EQ(box.minExtent()[2], 1);

    box.maxExtent() = glm::vec3(2,2,2);
    EXPECT_EQ(box.maxExtent()[0], 2);
    EXPECT_EQ(box.maxExtent()[1], 2);
    EXPECT_EQ(box.maxExtent()[2], 2);
}

//TODO
/*
TEST(Box, Intersect)
{
    Ray ray;
    Box box;
    std::array<float,2> t;
    bool success = true;
    success = Intersect(ray, box, t);
}
*/

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
