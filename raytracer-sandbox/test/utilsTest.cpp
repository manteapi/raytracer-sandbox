#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

#include <raytracer-sandbox/utils.hpp>

using namespace std;

TEST(Utils, Clamp)
{
    float minBound=0, maxBound=1;
    EXPECT_EQ(clamp(-0.5,minBound,maxBound),minBound);
    EXPECT_EQ(clamp(1.5,minBound,maxBound),maxBound);
    EXPECT_EQ(clamp(0.5,minBound,maxBound),0.5);
}

TEST(Utils, Fresnel)
{
    glm::vec3 incident, normal;
    float ior=1.0, kr, kt;

    //Force total internal reflection case
    incident = glm::vec3(0,0,-1);
    normal = glm::vec3(1,0,0);
    fresnel(incident, normal, ior, kr, kt);
    EXPECT_EQ(kr,1);
    EXPECT_EQ(kt,0);

    //Force total internal reflection case
    incident = glm::vec3(0,0,-1);
    normal = glm::vec3(0,0,1);
    fresnel(incident, normal, ior, kr, kt);
    EXPECT_EQ(kr,0);
    EXPECT_EQ(kt,1);
}

TEST(Utils, Refract)
{
    glm::vec3 incident, normal, refraction;
    float ior=1.0;
    incident = glm::vec3(0,0,-1);
    normal = glm::vec3(0,0,1);
    refraction = refract(incident, normal, ior);
    EXPECT_EQ(refraction[0], incident[0]);
    EXPECT_EQ(refraction[1], incident[1]);
    EXPECT_EQ(refraction[2], incident[2]);
}

TEST(Utils, Reflect)
{
    glm::vec3 incident, normal, reflection;
    incident = glm::vec3(0,-1,-1);
    normal = glm::vec3(0,0,1);
    reflection = reflect(incident, normal);
    EXPECT_EQ(reflection[0], incident[0]);
    EXPECT_EQ(reflection[1], incident[1]);
    EXPECT_EQ(reflection[2], -incident[2]);
}

TEST(Utils, VecStream)
{
    glm::vec3 v(1,2,3);
    ostringstream vStream1;
    vStream1 << v;
    ostringstream vStream2;
    vStream2 << v[0] << ", " << v[1] << ", " << v[2] << endl;
    string vString1 = vStream1.str();
    string vString2 = vStream2.str();
    EXPECT_EQ(vString1, vString2);
}

TEST(Utils, MatStream)
{
    glm::mat4 m(1.0);
    ostringstream mStream1;
    mStream1 << m;
    ostringstream mStream2;
    mStream2 << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << endl;
    mStream2 << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << endl;
    mStream2 << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << endl;
    mStream2 << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << endl;
    string mString1 = mStream1.str();
    string mString2 = mStream2.str();
    EXPECT_EQ(mString1, mString2);
}

TEST(Utils, SolveQuadratic)
{
    float a,b,c,x1,x2;
    bool success;

    a=1.0;
    b=1.0;
    c=-4;
    success = solveQuadratic(a,b,c,x1,x2);
    EXPECT_EQ(success, true);
    EXPECT_EQ(x1, (float)(-1+sqrt(17))/2);
    EXPECT_EQ(x2, (float)(-1-sqrt(17))/2);

    a=9.0;
    b=12.0;
    c=4.0;
    success = solveQuadratic(a,b,c,x1,x2);
    EXPECT_EQ(success, true);
    EXPECT_EQ(x1, (float)(-2.0/3.0));
    EXPECT_EQ(x2, (float)(-2.0/3.0));

    a=1;
    b=4.0;
    c=5.0;
    success = solveQuadratic(a,b,c,x1,x2);
    EXPECT_EQ(success, false);
}

TEST(Utils, PlaneRayIntersection)
{
    bool success;
    glm::vec3 pointInPlane, planeNormal, hitPosition, hitNormal, origin, direction;
    Ray ray;

    pointInPlane = glm::vec3(0,0,0);
    planeNormal = glm::vec3(0,0,1);
    origin = glm::vec3(0,0,1);
    direction = glm::vec3(0,0,-1);
    ray = Ray(origin, direction);
    success = planeRayIntersection(pointInPlane, planeNormal, ray, hitPosition, hitNormal);
    EXPECT_EQ(success, true);
    EXPECT_EQ(hitPosition[0], 0);
    EXPECT_EQ(hitPosition[1], 0);
    EXPECT_EQ(hitPosition[2], 0);
    EXPECT_EQ(hitNormal[0], 0);
    EXPECT_EQ(hitNormal[1], 0);
    EXPECT_EQ(hitNormal[2], 1);

    direction = glm::vec3(1,0,0);
    ray = Ray(origin, direction);
    success = planeRayIntersection(pointInPlane, planeNormal, ray, hitPosition, hitNormal);
    EXPECT_EQ(success, false);
}

TEST(Utils, Barycentric)
{
    glm::vec3 p(0,0.5,0);
    glm::vec3 a(-0.5,0,0), b(0.5,0,0), c(0,1,0);
    float u, v, w;
    barycentric(p, a, b, c, u, v, w);
    EXPECT_EQ(u, 0.25);
    EXPECT_EQ(v, 0.25);
    EXPECT_EQ(w, 0.5);
}

TEST(Utils, TriangleRayIntersection)
{
    bool success;
    glm::vec3 t1, t2, t3, hitPosition, hitNormal, barycentricCoords, origin, direction;
    Ray ray;

    //Ray does not intersect the plane made by the triangle
    t1 = glm::vec3(-0.5,0,0);
    t2 = glm::vec3(0.5,0,0);
    t3 = glm::vec3(0,1,0);
    origin = glm::vec3(1.0,0.0,1.0);
    direction = glm::vec3(1.0,0.0,0.0);
    ray = Ray(origin, direction);
    success = triangleRayIntersection(t1, t2, t3, ray, hitPosition, hitNormal, barycentricCoords);
    EXPECT_EQ(success, false);

    //Ray does not intersect the triangle
    t1 = glm::vec3(-0.5,0,0);
    t2 = glm::vec3(0.5,0,0);
    t3 = glm::vec3(0,1,0);
    origin = glm::vec3(1.0,0.0,1.0);
    direction = glm::vec3(0.0,0.0,-1.0);
    ray = Ray(origin, direction);
    success = triangleRayIntersection(t1, t2, t3, ray, hitPosition, hitNormal, barycentricCoords);
    EXPECT_EQ(success, false);

    //Ray does intersect the triangle
    t1 = glm::vec3(-0.5,0,0);
    t2 = glm::vec3(0.5,0,0);
    t3 = glm::vec3(0,1,0);
    origin = glm::vec3(0.0,0.5,1.0);
    direction = glm::vec3(0.0,0.0,-1.0);
    ray = Ray(origin, direction);
    success = triangleRayIntersection(t1, t2, t3, ray, hitPosition, hitNormal, barycentricCoords);
    EXPECT_EQ(success, true);
    EXPECT_EQ(barycentricCoords[0], 0.25);
    EXPECT_EQ(barycentricCoords[1], 0.25);
    EXPECT_EQ(barycentricCoords[2], 0.5);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
