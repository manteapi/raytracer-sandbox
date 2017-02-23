#include <iostream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/material.hpp>

using namespace std;

TEST(GlossyMaterial, Constructor)
{
    bool checkType = false;

    //Default constructor
    GlossyMaterial defaultMaterial;
    checkType = defaultMaterial.type() == MaterialType::GLOSSY;
    EXPECT_EQ(checkType,true);

    //Clone constructor
    GlossyMaterial cloneMaterial(defaultMaterial);
    checkType = cloneMaterial.type() == MaterialType::GLOSSY;
    EXPECT_EQ(checkType,true);
}

TEST(FresnelMaterial, Constructor)
{
    bool checkType = false;

    //Default constructor
    FresnelMaterial defaultMaterial;
    checkType = defaultMaterial.type() == MaterialType::FRESNEL;
    EXPECT_EQ(checkType,true);
    EXPECT_EQ(defaultMaterial.ior(), 0.0f);

    //Specific constructor
    float pi=3.141592;
    FresnelMaterial specificMaterial(pi);
    checkType = specificMaterial.type() == MaterialType::FRESNEL;
    EXPECT_EQ(checkType,true);
    EXPECT_EQ(specificMaterial.ior(), pi);

    //Clone constructor
    FresnelMaterial cloneMaterial(specificMaterial);
    checkType = cloneMaterial.type() == MaterialType::FRESNEL;
    EXPECT_EQ(checkType,true);
    EXPECT_EQ(cloneMaterial.ior(), pi);
}

TEST(FresnelMaterial, GetterSetter)
{
    float pi=3.141592;
    float pi2=pi*pi;
    FresnelMaterial material(pi);
    const FresnelMaterial constMaterial(pi);
    //Getter
    const float & materialIOR = constMaterial.ior();
    EXPECT_EQ(materialIOR,pi);
    //Setter
    material.ior() = pi2;
    EXPECT_EQ(material.ior(),pi2);
}

TEST(FresnelMaterial, IORValues)
{
    EXPECT_EQ(FresnelMaterial::AirIOR(), 1.0f);
    EXPECT_EQ(FresnelMaterial::WaterIOR(), 1.3f);
    EXPECT_EQ(FresnelMaterial::GlassIOR(), 1.5f);
    EXPECT_EQ(FresnelMaterial::DiamondIOR(), 1.8f);
}

TEST(PhongMaterial, Constructor)
{
    bool checkType = false;

    //Default constructor
    PhongMaterial defaultMaterial;
    checkType = defaultMaterial.type() == MaterialType::PHONG;
    EXPECT_EQ(checkType, true);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(defaultMaterial.ambient()[i], 0);
        EXPECT_EQ(defaultMaterial.diffuse()[i], 0);
        EXPECT_EQ(defaultMaterial.specular()[i], 0);
    }
    EXPECT_EQ(defaultMaterial.shininess(),0);

    //Specific constructor
    float pi=3.141592;
    glm::vec3 ambient(pi,2*pi,3*pi);
    glm::vec3 diffuse(4*pi,5*pi,6*pi);
    glm::vec3 specular(7*pi, 8*pi, 9*pi);
    float shininess=10*pi;
    PhongMaterial specificMaterial(ambient, diffuse, specular, shininess);
    checkType = specificMaterial.type() == MaterialType::PHONG;
    EXPECT_EQ(checkType, true);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(specificMaterial.ambient()[i], ambient[i]);
        EXPECT_EQ(specificMaterial.diffuse()[i], diffuse[i]);
        EXPECT_EQ(specificMaterial.specular()[i], specular[i]);
    }
    EXPECT_EQ(specificMaterial.shininess(), shininess);

    //Clone constructor
    PhongMaterial cloneMaterial(specificMaterial);
    checkType = cloneMaterial.type() == MaterialType::PHONG;
    EXPECT_EQ(checkType, true);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(cloneMaterial.ambient()[i], specificMaterial.ambient()[i]);
        EXPECT_EQ(cloneMaterial.diffuse()[i], specificMaterial.diffuse()[i]);
        EXPECT_EQ(cloneMaterial.specular()[i], specificMaterial.specular()[i]);
    }
    EXPECT_EQ(cloneMaterial.shininess(), specificMaterial.shininess());
}

TEST(PhongMaterial, GetterSetter)
{
    float pi=3.141592;
    glm::vec3 ambient(pi,2*pi,3*pi);
    glm::vec3 diffuse(4*pi,5*pi,6*pi);
    glm::vec3 specular(7*pi, 8*pi, 9*pi);
    float shininess=10*pi;

    //Getter
    PhongMaterial specificMaterial(ambient, diffuse, specular, shininess);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(specificMaterial.ambient()[i], ambient[i]);
        EXPECT_EQ(specificMaterial.diffuse()[i], diffuse[i]);
        EXPECT_EQ(specificMaterial.specular()[i], specular[i]);
    }
    EXPECT_EQ(specificMaterial.shininess(), shininess);

    //Setter
    float phi=1.618;
    glm::vec3 ambient2(phi,2*phi,3*phi);
    glm::vec3 diffuse2(4*phi,5*phi,6*phi);
    glm::vec3 specular2(7*phi, 8*phi, 9*phi);
    float shininess2=10*phi;

    specificMaterial.setAmbient(ambient2);
    specificMaterial.setDiffuse(diffuse2);
    specificMaterial.setSpecular(specular2);
    specificMaterial.setShininess(shininess2);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(specificMaterial.ambient()[i], ambient2[i]);
        EXPECT_EQ(specificMaterial.diffuse()[i], diffuse2[i]);
        EXPECT_EQ(specificMaterial.specular()[i], specular2[i]);
    }
    EXPECT_EQ(specificMaterial.shininess(), shininess2);
}

TEST(PhongMaterial, MaterialConfig)
{
    float openGLFactor=128.0;

    PhongMaterialPtr pearl = PhongMaterial::Pearl();
    EXPECT_EQ( pearl->ambient()[0], 0.25f );
    EXPECT_EQ( pearl->ambient()[1], 0.20725f );
    EXPECT_EQ( pearl->ambient()[2], 0.20725f );
    EXPECT_EQ( pearl->diffuse()[0], 1.0f );
    EXPECT_EQ( pearl->diffuse()[1], 0.829f);
    EXPECT_EQ( pearl->diffuse()[2], 0.829f );
    EXPECT_EQ( pearl->specular()[0], 0.296648f );
    EXPECT_EQ( pearl->specular()[1], 0.296648f );
    EXPECT_EQ( pearl->specular()[2], 0.296648f );
    EXPECT_EQ( pearl->shininess(), openGLFactor*0.088f );

    PhongMaterialPtr bronze = PhongMaterial::Bronze();
    EXPECT_EQ( bronze->ambient()[0], 0.2125f );
    EXPECT_EQ( bronze->ambient()[1], 0.1275f );
    EXPECT_EQ( bronze->ambient()[2], 0.054f );
    EXPECT_EQ( bronze->diffuse()[0], 0.714f );
    EXPECT_EQ( bronze->diffuse()[1], 0.4284f);
    EXPECT_EQ( bronze->diffuse()[2], 0.18144f );
    EXPECT_EQ( bronze->specular()[0], 0.393548f );
    EXPECT_EQ( bronze->specular()[1], 0.271906f );
    EXPECT_EQ( bronze->specular()[2], 0.166721f );
    EXPECT_EQ( bronze->shininess(), openGLFactor*0.2f );

    PhongMaterialPtr emerald = PhongMaterial::Emerald();
    EXPECT_EQ( emerald->ambient()[0], 0.0215f );
    EXPECT_EQ( emerald->ambient()[1], 0.1745f );
    EXPECT_EQ( emerald->ambient()[2], 0.0215f );
    EXPECT_EQ( emerald->diffuse()[0], 0.07568f );
    EXPECT_EQ( emerald->diffuse()[1], 0.61424f);
    EXPECT_EQ( emerald->diffuse()[2], 0.07568f );
    EXPECT_EQ( emerald->specular()[0], 0.633f );
    EXPECT_EQ( emerald->specular()[1], 0.727811f );
    EXPECT_EQ( emerald->specular()[2], 0.633f );
    EXPECT_EQ( emerald->shininess(), openGLFactor*0.6f );
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
