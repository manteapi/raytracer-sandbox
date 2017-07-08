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

    glm::vec3 minExtent = constBox.minBound();
    EXPECT_EQ(minExtent[0], 0);
    EXPECT_EQ(minExtent[1], 0);
    EXPECT_EQ(minExtent[2], 0);

    glm::vec3 maxExtent = constBox.maxBound();
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

    box.minBound() = glm::vec3(1,1,1);
    EXPECT_EQ(box.minBound()[0], 1);
    EXPECT_EQ(box.minBound()[1], 1);
    EXPECT_EQ(box.minBound()[2], 1);

    box.maxBound() = glm::vec3(2,2,2);
    EXPECT_EQ(box.maxBound()[0], 2);
    EXPECT_EQ(box.maxBound()[1], 2);
    EXPECT_EQ(box.maxBound()[2], 2);
}

TEST(Box, Intersect)
{
    glm::vec3 minBB, maxBB, origin, direction;
    Ray ray;
    Box box;
    std::array<float,2> t;
    bool success = false;

    //Case 0 : No Intersection
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(-0.5,-0.5,-0.5);
    direction = glm::vec3(0,0,1);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, false);

    //Case 1 : No Intersection
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(-0.5,-0.5,1.5);
    direction = glm::vec3(0,0,1);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, false);

    //Case 2 : No Intersection
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(-0.5,-0.5,-0.5);
    direction = glm::vec3(1,0,0);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, false);

    //Case 3 : No Intersection
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(1.5,-0.5,-0.5);
    direction = glm::vec3(1,0,0);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, false);

    //Case 3 : Intersection along z-axis
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(0.5,0.5,-0.5);
    direction = glm::vec3(0,0,1);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, true);
    EXPECT_EQ(t[0], 0.5);
    EXPECT_EQ(t[1], 1.5);

    //Case 4 : Intersection along -z-axis
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(0.5,0.5,1.5);
    direction = glm::vec3(0,0,1);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, true);
    EXPECT_EQ(t[0], -1.5);
    EXPECT_EQ(t[1], -0.5);

    //Case 5 : Intersection along x-axis
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(-0.5,0.5,0.5);
    direction = glm::vec3(1,0,0);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, true);
    EXPECT_EQ(t[0], 0.5);
    EXPECT_EQ(t[1], 1.5);

    //Case 6 : Intersection along -x-axis
    minBB = glm::vec3(0,0,0);
    maxBB = glm::vec3(1,1,1);
    origin = glm::vec3(1.5,0.5,0.5);
    direction = glm::vec3(1,0,0);
    box = Box(minBB, maxBB);
    ray = Ray(origin, direction);
    success = Intersect(ray, box, t);
    EXPECT_EQ(success, true);
    EXPECT_EQ(t[0], -1.5);
    EXPECT_EQ(t[1], -0.5);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
