#include <iostream>
#include <gtest/gtest.h>
#include <raytracer-sandbox/plane.hpp>

using namespace std;

TEST(Plane, Constructor_Normal)
{
    glm::vec3 normal(0,0,1), point(1,1,1);
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Plane plane(normal, point, material);

    //Check object's properties : bbox, material
    Box planeBox = plane.bbox();
    EXPECT_EQ(planeBox.minExtent()[0], -std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.minExtent()[1], -std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.minExtent()[2], -std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.maxExtent()[0], std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.maxExtent()[1], std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.maxExtent()[2], std::numeric_limits<float>::max());

    MaterialPtr planeMaterial = plane.material();
    EXPECT_EQ(planeMaterial->type(), MaterialType::PHONG);
    PhongMaterialPtr planePhongMaterial = static_pointer_cast<PhongMaterial>(planeMaterial);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(planePhongMaterial->ambient()[i], material->ambient()[i]);
        EXPECT_EQ(planePhongMaterial->diffuse()[i], material->diffuse()[i]);
        EXPECT_EQ(planePhongMaterial->specular()[i], material->specular()[i]);
    }
    EXPECT_EQ(planePhongMaterial->shininess(), material->shininess());

    //Check plane's properties : normal, distanceToOrigin
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(plane.normal()[i], normal[i]);
    }
    EXPECT_EQ(plane.distanceToOrigin(), glm::dot(normal, point));
}

TEST(Plane, Constructor_Points)
{
    glm::vec3 p1(1,1,1), p2(0,1,1), p3(1,0,1);
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Plane plane(p1,p2,p3, material);

    //Check object's properties : bbox, material
    Box planeBox = plane.bbox();
    EXPECT_EQ(planeBox.minExtent()[0], -std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.minExtent()[1], -std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.minExtent()[2], -std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.maxExtent()[0], std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.maxExtent()[1], std::numeric_limits<float>::max());
    EXPECT_EQ(planeBox.maxExtent()[2], std::numeric_limits<float>::max());

    MaterialPtr planeMaterial = plane.material();
    EXPECT_EQ(planeMaterial->type(), MaterialType::PHONG);
    PhongMaterialPtr planePhongMaterial = static_pointer_cast<PhongMaterial>(planeMaterial);
    for(int i=0; i<3; ++i)
    {
        EXPECT_EQ(planePhongMaterial->ambient()[i], material->ambient()[i]);
        EXPECT_EQ(planePhongMaterial->diffuse()[i], material->diffuse()[i]);
        EXPECT_EQ(planePhongMaterial->specular()[i], material->specular()[i]);
    }
    EXPECT_EQ(planePhongMaterial->shininess(), material->shininess());

    //Check plane's properties : normal, distanceToOrigin
    EXPECT_EQ(plane.normal()[0], 0);
    EXPECT_EQ(plane.normal()[1], 0);
    EXPECT_EQ(plane.normal()[2], 1);
    EXPECT_EQ(plane.distanceToOrigin(), glm::dot(glm::vec3(0,0,1), p1));
}

TEST(Plane, Setter)
{
    Plane plane;

    plane.setDistanceToOrigin(10.0);
    EXPECT_EQ(plane.distanceToOrigin(), 10.0);

    plane.setNormal(glm::vec3(1,0,0));
    EXPECT_EQ(plane.normal()[0], 1);
    EXPECT_EQ(plane.normal()[1], 0);
    EXPECT_EQ(plane.normal()[2], 0);
}

TEST(Plane, ProjectOnPlane)
{
    glm::vec3 normal(0,0,1), point(0,0,0);
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Plane plane(normal, point, material);

    glm::vec3 projection = plane.projectOnPlane(glm::vec3(10,10,10));
    EXPECT_EQ(projection[0], 10);
    EXPECT_EQ(projection[1], 10);
    EXPECT_EQ(projection[2], 0);
}

TEST(Plane, Intersect)
{
    bool success = false;
    glm::vec3 normal(0,0,1), point(0,0,0), origin, direction, hitPosition, hitNormal;
    Ray ray;
    PhongMaterialPtr material = PhongMaterial::Bronze();
    const Plane plane(normal, point, material);

    //Case 1 : Intersection
    origin = glm::vec3(10,10,10);
    direction = glm::vec3(0,0,-1);
    ray = Ray(origin, direction);
    success = plane.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, true);
    EXPECT_EQ(hitPosition[0], 10);
    EXPECT_EQ(hitPosition[1], 10);
    EXPECT_EQ(hitPosition[2], 0);
    EXPECT_EQ(hitNormal[0], plane.normal()[0]);
    EXPECT_EQ(hitNormal[1], plane.normal()[1]);
    EXPECT_EQ(hitNormal[2], plane.normal()[2]);

    //Case 2 : No Intersection - The ray does not go in the right direction
    origin = glm::vec3(10,10,10);
    direction = glm::vec3(0,0,1);
    ray = Ray(origin, direction);
    success = plane.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, false);

    //Case 3 : No Intersection - The ray is perpendicular to the plane
    origin = glm::vec3(10,10,10);
    direction = glm::vec3(1,0,0);
    ray = Ray(origin, direction);
    success = plane.Intersect(ray, hitPosition, hitNormal);
    EXPECT_EQ(success, false);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
