#include <iostream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/io.hpp>
#include "config.h"

TEST(IO, read_obj)
{

    std::vector<glm::vec3> positions, normals;
    std::vector<unsigned int> triangles;
    std::vector<glm::vec2> texcoords;
    std::string filename;
    bool success = true;

    filename = "nonepath.obj";
    success = read_obj(filename, positions, triangles, normals, texcoords);
    EXPECT_EQ(success, false);

    filename = CurrentBinaryDir()+"/../test/meshes/triangle.obj";
    std::cout << filename << std::endl;
    success = read_obj(filename, positions, triangles, normals, texcoords);
    EXPECT_EQ(success, true);
    EXPECT_EQ(positions.size(), size_t(3));
    EXPECT_EQ(triangles.size(), size_t(3));
    EXPECT_EQ(normals.size(), size_t(3));
    EXPECT_EQ(texcoords.size(), size_t(3));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
