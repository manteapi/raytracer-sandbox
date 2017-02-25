#include <iostream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/tmesh.hpp>
#include "config.h"

using namespace std;

TEST(TMesh, Constructor)
{
    string filename = CurrentBinaryDir()+"/../test/meshes/triangle.obj";
    PhongMaterialPtr material = PhongMaterial::Bronze();
    TMesh mesh(filename, material);

    Box meshBox = mesh.bbox();
    EXPECT_EQ(meshBox.minExtent()[0], -0.5);
    EXPECT_EQ(meshBox.minExtent()[1], -0.5);
    EXPECT_EQ(meshBox.minExtent()[2], 0);
    EXPECT_EQ(meshBox.maxExtent()[0], 0.5);
    EXPECT_EQ(meshBox.maxExtent()[1], 0.5);
    EXPECT_EQ(meshBox.maxExtent()[2], 0);

    MaterialPtr meshMaterial = mesh.material();
    EXPECT_EQ(meshMaterial->type(), MaterialType::PHONG);
    PhongMaterialPtr meshPhongMaterial = static_pointer_cast<PhongMaterial>(meshMaterial);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(meshPhongMaterial->ambient()[i], material->ambient()[i]);
        EXPECT_EQ(meshPhongMaterial->diffuse()[i], material->diffuse()[i]);
        EXPECT_EQ(meshPhongMaterial->specular()[i], material->specular()[i]);
    }
    EXPECT_EQ(meshPhongMaterial->shininess(), material->shininess());
}

TEST(TMesh, Intersect)
{
    string filename = CurrentBinaryDir()+"/../test/meshes/triangle.obj";
    PhongMaterialPtr material = PhongMaterial::Bronze();
    TMesh mesh(filename, material);

    bool success = false;
    glm::vec3 origin, direction, hitPosition, hitNormal;
    Ray ray;

    origin = glm::vec3(1.0,1.0,-0.5);
    direction = glm::vec3(0.0,0.0,1.0);
    ray = Ray(origin, direction);
    success = mesh.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, false);

    origin = glm::vec3(0.0,0.0,-0.5);
    direction = glm::vec3(0.0,0.0,1.0);
    ray = Ray(origin, direction);
    success = mesh.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, true);
    EXPECT_EQ(hitPosition[0], 0);
    EXPECT_EQ(hitPosition[1], 0);
    EXPECT_EQ(hitPosition[2], 0);
    EXPECT_EQ(hitNormal[0], 0);
    EXPECT_EQ(hitNormal[1], 0);
    EXPECT_EQ(hitNormal[2], 1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
