#include <iostream>
#include <gtest/gtest.h>
#include <raytracer-sandbox/spotLight.hpp>
#include <raytracer-sandbox/utils.hpp>

using namespace std;

TEST(SpotLight, Constructor)
{
    float constant=1.0, linear=1.0, quadratic=1.0;
    float innerCutOff = cos(glm::radians(8.0f)), outerCutOff = cos(glm::radians(16.0f));
    glm::vec3 position(0,0,0), spotDirection(0,0,1);
    glm::vec3 ambient(1.0,0.0,1.0), diffuse(0.0,1.0,0.0), specular(1.0,1.0,1.0);
    const SpotLight light(position, spotDirection, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(light.position()[i], position[i]);
        EXPECT_EQ(light.spotDirection()[i], spotDirection[i]);
        EXPECT_EQ(light.ambient()[i], ambient[i]);
        EXPECT_EQ(light.diffuse()[i], diffuse[i]);
        EXPECT_EQ(light.specular()[i], specular[i]);
    }
    EXPECT_EQ(light.constant(), constant);
    EXPECT_EQ(light.linear(), linear);
    EXPECT_EQ(light.quadratic(), quadratic);
    EXPECT_EQ(light.innerCutOff(), innerCutOff);
    EXPECT_EQ(light.outerCutOff(), outerCutOff);
}

TEST(SpotLight, Setter)
{
    float constant=1.0, linear=1.0, quadratic=1.0;
    float innerCutOff = cos(glm::radians(8.0f)), outerCutOff = cos(glm::radians(16.0f));
    glm::vec3 position(0,0,0), spotDirection(0,0,1);
    glm::vec3 ambient(1.0,0.0,1.0), diffuse(0.0,1.0,0.0), specular(1.0,1.0,1.0);

    SpotLight light;
    light.setPosition(position);
    light.setSpotDirection(spotDirection);
    light.setAmbient(ambient);
    light.setDiffuse(diffuse);
    light.setSpecular(specular);
    light.setConstant(constant);
    light.setLinear(linear);
    light.setQuadratic(quadratic);
    light.setInnerCutOff(innerCutOff);
    light.setOuterCutOff(outerCutOff);

    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(light.position()[i], position[i]);
        EXPECT_EQ(light.spotDirection()[i], spotDirection[i]);
        EXPECT_EQ(light.ambient()[i], ambient[i]);
        EXPECT_EQ(light.diffuse()[i], diffuse[i]);
        EXPECT_EQ(light.specular()[i], specular[i]);
    }
    EXPECT_EQ(light.constant(), constant);
    EXPECT_EQ(light.linear(), linear);
    EXPECT_EQ(light.quadratic(), quadratic);
    EXPECT_EQ(light.innerCutOff(), innerCutOff);
    EXPECT_EQ(light.outerCutOff(), outerCutOff);
}

TEST(SpotLight, LightFromDirection)
{
    float constant=1.0, linear=1.0, quadratic=1.0;
    float innerCutOff = cos(glm::radians(8.0f)), outerCutOff = cos(glm::radians(16.0f));
    glm::vec3 position(0,0,1), spotDirection(0,0,1), ambient(1.0,0.0,1.0), diffuse(0.0,1.0,0.0), specular(1.0,1.0,1.0);
    const SpotLight light(position, spotDirection, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff);
    EXPECT_EQ(light.lightDirectionFrom(glm::vec3(0,0,0))[0], glm::normalize(-light.position())[0]);
    EXPECT_EQ(light.lightDirectionFrom(glm::vec3(0,0,0))[1], glm::normalize(-light.position())[1]);
    EXPECT_EQ(light.lightDirectionFrom(glm::vec3(0,0,0))[2], glm::normalize(-light.position())[2]);
}

TEST(SpotLight, PhongIllumination)
{
    float constant=1.0, linear=0.0, quadratic=0.0;
    float innerCutOff = cos(glm::radians(8.0f)), outerCutOff = cos(glm::radians(16.0f));
    glm::vec3 position(0,0,-2), spotDirection(0,0,1);
    glm::vec3 color(0,0,0), eyePosition(0,0,-1), surfacePosition(0,0,0), surfaceNormal(0,0,-1);
    glm::vec3 ambient(1.0,1.0,1.0), diffuse(1.0,1.0,1.0), specular(1.0,1.0,1.0);
    const SpotLight light(position, spotDirection, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff);

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
