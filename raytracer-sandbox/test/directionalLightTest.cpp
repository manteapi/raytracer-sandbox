#include <iostream>
#include <gtest/gtest.h>
#include <raytracer-sandbox/directionalLight.hpp>
#include <raytracer-sandbox/utils.hpp>

using namespace std;

TEST(DirectionalLight, Constructor)
{
    glm::vec3 direction(0,0,1), ambient(1.0,0.0,1.0), diffuse(0.0,1.0,0.0), specular(1.0,1.0,1.0);
    const DirectionalLight light(direction, ambient, diffuse, specular);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(light.ambient()[i], ambient[i]);
        EXPECT_EQ(light.diffuse()[i], diffuse[i]);
        EXPECT_EQ(light.specular()[i], specular[i]);
        EXPECT_EQ(light.direction()[i], direction[i]);
    }
}

TEST(DirectionalLight, Setter)
{
    glm::vec3 direction(0,0,1), ambient(1.0,0.0,1.0), diffuse(0.0,1.0,0.0), specular(1.0,1.0,1.0);
    DirectionalLight light;
    light.setAmbient(ambient);
    light.setDiffuse(diffuse);
    light.setSpecular(specular);
    light.setDirection(direction);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(light.ambient()[i], ambient[i]);
        EXPECT_EQ(light.diffuse()[i], diffuse[i]);
        EXPECT_EQ(light.specular()[i], specular[i]);
        EXPECT_EQ(light.direction()[i], direction[i]);
    }
}

TEST(DirectionalLight, LightFromDirection)
{
    glm::vec3 direction(0,0,1), ambient(1.0,0.0,1.0), diffuse(0.0,1.0,0.0), specular(1.0,1.0,1.0);
    const DirectionalLight light(direction, ambient, diffuse, specular);
    EXPECT_EQ(light.lightDirectionFrom(glm::vec3(0,0,0))[0], direction[0]);
    EXPECT_EQ(light.lightDirectionFrom(glm::vec3(0,0,0))[1], direction[1]);
    EXPECT_EQ(light.lightDirectionFrom(glm::vec3(0,0,0))[2], direction[2]);
}

TEST(DirectionalLight, PhongIllumination)
{
    glm::vec3 color(0,0,0), eyePosition(0,0,-1), surfacePosition(0,0,0), surfaceNormal(0,0,-1), direction(0,0,1);
    glm::vec3 ambient(1.0,1.0,1.0), diffuse(1.0,1.0,1.0), specular(1.0,1.0,1.0);
    const DirectionalLight light(direction, ambient, diffuse, specular);

    glm::vec3 ambientMat(1.0,1,1.0), diffuseMat(1,1.0,1), specularMat(1.0,1.0,1.0);
    float shininessMat = 1.0;
    PhongMaterialPtr material = std::make_shared<PhongMaterial>(ambientMat, diffuseMat, specularMat, shininessMat);
    color = light.phongIllumination(eyePosition, surfacePosition, surfaceNormal, material);

    EXPECT_EQ(color[0], 3);
    EXPECT_EQ(color[1], 3);
    EXPECT_EQ(color[2], 3);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
