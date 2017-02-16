#ifndef PATHTRACING_HPP
#define PATHTRACING_HPP

#include "ray.hpp"
#include "object.hpp"
#include "light.hpp"
#include <QVector3D>

bool pathTrace(const Ray& viewRay, const std::vector<ObjectPtr>& objects, ObjectPtr &closestHitObject, QVector3D& closestHitPosition, QVector3D& closestHitNormal);
QVector3D castRay(const Ray& ray, const std::vector<LightPtr> &lights, const std::vector<ObjectPtr> &objects,
                  const QVector3D& backgroundColor, const QVector3D& shadowColor, const float& bias, const int &maxDepth, int depth);

#endif //PATHTRACING_HPP
