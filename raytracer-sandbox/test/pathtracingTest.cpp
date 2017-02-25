#include <iostream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/sphere.hpp>
#include <raytracer-sandbox/pathtracing.hpp>
#include <raytracer-sandbox/directionalLight.hpp>

TEST(Pathtracing, Pathtrace)
{
    bool success;
    float radius;
    glm::vec3 origin, direction, position, closestHitPosition, closestHitNormal;
    Ray viewRay;
    SpherePtr sphere;
    ObjectPtr closestHitObject;
    PhongMaterialPtr material = PhongMaterial::Bronze();
    std::vector<ObjectPtr> objects;

    //Case 1 : Intersection occurs
    origin = glm::vec3(0,0,-2);
    direction = glm::vec3(0,0,1);
    position = glm::vec3(0,0,0);
    radius = 1.0f;
    viewRay = Ray(origin, direction);
    sphere = std::make_shared<Sphere>(position, radius, material);
    objects.push_back( sphere );
    success = pathTrace(viewRay, objects, closestHitObject, closestHitPosition, closestHitNormal);
    EXPECT_EQ(success, true);
    EXPECT_EQ(closestHitObject, sphere);
    EXPECT_EQ(closestHitPosition[0], 0);
    EXPECT_EQ(closestHitPosition[1], 0);
    EXPECT_EQ(closestHitPosition[2], -1);

    //Case 2 : Intersection does not occur
    objects.clear();
    origin = glm::vec3(-2,-2,-2);
    direction = glm::vec3(0,0,1);
    position = glm::vec3(0,0,0);
    radius = 1.0f;
    viewRay = Ray(origin, direction);
    sphere = std::make_shared<Sphere>(position, radius, material);
    objects.push_back( sphere );
    success = pathTrace(viewRay, objects, closestHitObject, closestHitPosition, closestHitNormal);
    EXPECT_EQ(success, false);
}

TEST(Pathtracing, CastRay_Phong)
{
    //Material
    glm::vec3 ambientMat(1.0,1,1.0), diffuseMat(1,1.0,1), specularMat(1.0,1.0,1.0);
    float shininessMat = 1.0;
    PhongMaterialPtr material = std::make_shared<PhongMaterial>(ambientMat, diffuseMat, specularMat, shininessMat);

    //Objects
    float radius = 1.0f;
    glm::vec3 position(0,0,0);
    SpherePtr sphere = std::make_shared<Sphere>(position, radius, material);
    std::vector<ObjectPtr> objects;
    objects.push_back( sphere );

    //Lights
    glm::vec3 lightDirection(0,0,1), ambient(1.0,1.0,1.0), diffuse(1.0,1.0,1.0), specular(1.0,1.0,1.0);
    std::vector<LightPtr> lights;
    DirectionalLightPtr dLight = std::make_shared<DirectionalLight>(lightDirection, ambient, diffuse, specular);
    lights.push_back(dLight);

    //Ray
    glm::vec3 origin(0,0,-2), rayDirection(0,0,1);
    Ray viewRay(origin, rayDirection);
    int depth=0, maxDepth=4;
    float bias = 1e-3;
    glm::vec3 backgroundColor(0,0,0), shadowColor(0.5,0.5,0.5);
    glm::vec3 color = castRay(viewRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth);

    EXPECT_EQ(color[0], 3);
    EXPECT_EQ(color[1], 3);
    EXPECT_EQ(color[2], 3);
}

TEST(Pathtracing, CastRay_Glossy)
{
    //Material
    GlossyMaterialPtr material = std::make_shared<GlossyMaterial>();

    //Objects
    float radius = 1.0f;
    glm::vec3 position(0,0,0);
    SpherePtr sphere = std::make_shared<Sphere>(position, radius, material);
    std::vector<ObjectPtr> objects;
    objects.push_back( sphere );

    //Lights
    glm::vec3 lightDirection(0,0,1), ambient(1.0,1.0,1.0), diffuse(1.0,1.0,1.0), specular(1.0,1.0,1.0);
    std::vector<LightPtr> lights;
    DirectionalLightPtr dLight = std::make_shared<DirectionalLight>(lightDirection, ambient, diffuse, specular);
    lights.push_back(dLight);

    //Ray
    glm::vec3 origin(0,0,-2), rayDirection(0,0,1);
    Ray viewRay(origin, rayDirection);
    int depth=0, maxDepth=4;
    float bias = 1e-3;
    glm::vec3 backgroundColor(0.2,0.2,0.2), shadowColor(0.5,0.5,0.5);
    glm::vec3 color = castRay(viewRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth);

    EXPECT_EQ(color[0], backgroundColor[0]);
    EXPECT_EQ(color[1], backgroundColor[1]);
    EXPECT_EQ(color[2], backgroundColor[2]);
}

TEST(Pathtracing, CastRay_Fresnel)
{
    //Material
    float ior = 1.0f;
    FresnelMaterialPtr material = std::make_shared<FresnelMaterial>(ior);

    //Objects
    float radius = 1.0f;
    glm::vec3 position(0,0,0);
    SpherePtr sphere = std::make_shared<Sphere>(position, radius, material);
    std::vector<ObjectPtr> objects;
    objects.push_back( sphere );

    //Lights
    glm::vec3 lightDirection(0,0,1), ambient(1.0,1.0,1.0), diffuse(1.0,1.0,1.0), specular(1.0,1.0,1.0);
    std::vector<LightPtr> lights;
    DirectionalLightPtr dLight = std::make_shared<DirectionalLight>(lightDirection, ambient, diffuse, specular);
    lights.push_back(dLight);

    //Ray
    glm::vec3 origin(0,0,-2), rayDirection(0,0,1);
    Ray viewRay(origin, rayDirection);
    int depth=0, maxDepth=4;
    float bias = 1e-3;
    glm::vec3 backgroundColor(0.2,0.2,0.2), shadowColor(0.5,0.5,0.5);
    glm::vec3 color = castRay(viewRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth);

    EXPECT_EQ(color[0], backgroundColor[0]);
    EXPECT_EQ(color[1], backgroundColor[1]);
    EXPECT_EQ(color[2], backgroundColor[2]);
}

TEST(Pathtracing, CastRay_Default)
{
    //Material
    MaterialPtr material = std::make_shared<Material>();

    //Objects
    float radius = 1.0f;
    glm::vec3 position(0,0,0);
    SpherePtr sphere = std::make_shared<Sphere>(position, radius, material);
    std::vector<ObjectPtr> objects;
    objects.push_back( sphere );

    //Lights
    glm::vec3 lightDirection(0,0,1), ambient(1.0,1.0,1.0), diffuse(1.0,1.0,1.0), specular(1.0,1.0,1.0);
    std::vector<LightPtr> lights;
    DirectionalLightPtr dLight = std::make_shared<DirectionalLight>(lightDirection, ambient, diffuse, specular);
    lights.push_back(dLight);

    //Ray
    glm::vec3 origin(0,0,-2), rayDirection(0,0,1);
    Ray viewRay(origin, rayDirection);
    int depth=0, maxDepth=4;
    float bias = 1e-3;
    glm::vec3 backgroundColor(0.2,0.2,0.2), shadowColor(0.5,0.5,0.5);
    glm::vec3 color = castRay(viewRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth);

    EXPECT_EQ(color[0], 0);
    EXPECT_EQ(color[1], 0);
    EXPECT_EQ(color[2], 0);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
