#ifndef UTILS_HPP
#define UTILS_HPP

#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>

#include "ray.hpp"

float clamp(float n, float lower, float upper);
QVector3D refract(const QVector3D &incident, const QVector3D &normal, const float &ior);
void fresnel(const QVector3D &incident, const QVector3D &normal, const float &ior, float &kr, float &kt);
QVector3D reflect(const QVector3D& incident, const QVector3D& normal);
bool solveQuadratic(const float& a, const float& b, const float&c, float& x1, float& x2);
bool planeRayIntersection(const QVector3D & pointInPlane, const QVector3D & planeNormal, const Ray & r, QVector3D & hitPosition, QVector3D & hitNormal);
void barycentric(const QVector3D& p, const QVector3D& a, const QVector3D& b, const QVector3D& c, float &u, float &v, float &w);
bool triangleRayIntersection(const QVector3D & t1, const QVector3D & t2, const QVector3D & t3,
                             const Ray & r, QVector3D & hitPosition, QVector3D & hitNormal, QVector3D & barycentricCoords);
QColor toColor(const QVector3D& c);

std::ostream& operator << ( std::ostream& out, const QVector3D& v);
std::ostream& operator << ( std::ostream& out, const QMatrix4x4& m);

#endif //UTILS_HPP
