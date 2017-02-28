#include <iostream>
#include <raytracer-sandbox/sphere.hpp>
#include <gtest/gtest.h>

using namespace std;

TEST(Sphere, Constructor)
{
    float radius=1.0;
    glm::vec3 position(0,0,0);
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Sphere sphere(position, radius, material);

    //Sphere's properties : radius and position
    EXPECT_EQ(sphere.radius(), radius);
    EXPECT_EQ(sphere.position()[0], position[0]);
    EXPECT_EQ(sphere.position()[1], position[1]);
    EXPECT_EQ(sphere.position()[2], position[2]);

    //Object's properties : bounding box and material
    EXPECT_EQ(sphere.bbox().minBound()[0], -1);
    EXPECT_EQ(sphere.bbox().minBound()[1], -1);
    EXPECT_EQ(sphere.bbox().minBound()[2], -1);
    EXPECT_EQ(sphere.bbox().maxBound()[0], 1);
    EXPECT_EQ(sphere.bbox().maxBound()[1], 1);
    EXPECT_EQ(sphere.bbox().maxBound()[2], 1);

    MaterialPtr sphereMaterial = sphere.material();
    EXPECT_EQ(sphereMaterial->type(), MaterialType::PHONG);
    PhongMaterialPtr spherePhongMaterial = static_pointer_cast<PhongMaterial>(sphereMaterial);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(spherePhongMaterial->ambient()[i], material->ambient()[i]);
        EXPECT_EQ(spherePhongMaterial->diffuse()[i], material->diffuse()[i]);
        EXPECT_EQ(spherePhongMaterial->specular()[i], material->specular()[i]);
    }
    EXPECT_EQ(spherePhongMaterial->shininess(), material->shininess());
}

TEST(Sphere, Setter)
{
    Sphere sphere;
    sphere.material() = PhongMaterial::Bronze();
}

TEST(Sphere, Intersect)
{
    bool success = false;
    float radius=1.0;
    glm::vec3 position(0,0,0), hitPosition, hitNormal, origin, direction;
    Ray ray;
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Sphere sphere(position, radius, material);

    //Case 1 : Intersection
    origin = glm::vec3(0.0,0.0,-2.0);
    direction = glm::vec3(0.0,0.0,1.0);
    ray = Ray(origin, direction);
    success = sphere.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, true);
    EXPECT_EQ(hitPosition[0], 0);
    EXPECT_EQ(hitPosition[1], 0);
    EXPECT_EQ(hitPosition[2], -1);

    //Case 2 : Intersection, the ray starts inside the sphere
    origin = glm::vec3(0.0,0.0,0.0);
    direction = glm::vec3(0.0,0.0,1.0);
    ray = Ray(origin, direction);
    success = sphere.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, true);
    EXPECT_EQ(hitPosition[0], 0);
    EXPECT_EQ(hitPosition[1], 0);
    EXPECT_EQ(hitPosition[2], 1);

    //Case 3 : No intersection, the ray does not go in the right direction
    origin = glm::vec3(0.0,0.0,-2.0);
    direction = glm::vec3(0.0,0.0,-1.0);
    ray = Ray(origin, direction);
    success = sphere.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, false);

    //Case 4 : No intersection
    origin = glm::vec3(-2.0,-2.0,-2.0);
    direction = glm::vec3(0.0,0.0,1.0);
    ray = Ray(origin, direction);
    success = sphere.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, false);
}

TEST(Sphere, SphereStream)
{
    float radius=1.0;
    glm::vec3 position(0,0,0);
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Sphere sphere(position, radius, material);

    ostringstream sStream1;
    sStream1 << sphere;

    ostringstream sStream2;
    sStream2 << "Radius : " << sphere.radius() << std::endl;
    sStream2 << "Position : " << sphere.position()[0] << ", " << sphere.position()[1] << ", " << sphere.position()[2] << std::endl;

    string sString1 = sStream1.str();
    string sString2 = sStream2.str();

    EXPECT_EQ(sString1, sString2);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
