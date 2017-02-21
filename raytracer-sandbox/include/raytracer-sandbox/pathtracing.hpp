#ifndef PATHTRACING_HPP
#define PATHTRACING_HPP

#include "ray.hpp"
#include "object.hpp"
#include "light.hpp"
#include <glm/glm.hpp>

bool pathTrace(const Ray& viewRay, const std::vector<ObjectPtr>& objects, ObjectPtr &closestHitObject, glm::vec3& closestHitPosition, glm::vec3& closestHitNormal);
glm::vec3 castRay(const Ray& ray, const std::vector<LightPtr> &lights, const std::vector<ObjectPtr> &objects,
                  const glm::vec3& backgroundColor, const glm::vec3& shadowColor, const float& bias, const int &maxDepth, int depth);

#endif //PATHTRACING_HPP
