#include <iostream>
#include <sstream>
#include <raytracer-sandbox/ray.hpp>
#include <gtest/gtest.h>

using namespace std;

TEST(Ray, Constructor)
{
    glm::vec3 origin(0,0,0), direction(0,0,1);
    const Ray ray(origin, direction);
    EXPECT_EQ(ray.origin()[0], 0);
    EXPECT_EQ(ray.origin()[1], 0);
    EXPECT_EQ(ray.origin()[2], 0);
    EXPECT_EQ(ray.direction()[0], 0);
    EXPECT_EQ(ray.direction()[1], 0);
    EXPECT_EQ(ray.direction()[2], 1);
    EXPECT_EQ(std::isinf(ray.invDirection()[0]), true);
    EXPECT_EQ(std::isinf(ray.invDirection()[1]), true);
    EXPECT_EQ(ray.invDirection()[2], 1);
    EXPECT_EQ(ray.sign()[0], 0);
    EXPECT_EQ(ray.sign()[1], 0);
    EXPECT_EQ(ray.sign()[2], 0);
}

TEST(Ray, RayStream)
{
    glm::vec3 origin(0,0,0), direction(0,0,1);
    const Ray ray(origin, direction);

    ostringstream rStream1;
    rStream1 << ray;

    ostringstream rStream2;
    rStream2 << "Direction : " << ray.direction()[0] << ", " << ray.direction()[1] << ", " << ray.direction()[2] << std::endl;
    rStream2 << "Origin : " << ray.origin()[0] << ", " << ray.origin()[1] << ", " << ray.origin()[2] << std::endl;

    string cString1 = rStream1.str();
    string cString2 = rStream2.str();

    EXPECT_EQ(cString1, cString2);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
