#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>

#include "ray.hpp"

float clamp(float n, float lower, float upper);
glm::vec3 refract(const glm::vec3 &incident, const glm::vec3 &normal, const float &ior);
void fresnel(const glm::vec3 &incident, const glm::vec3 &normal, const float &ior, float &kr, float &kt);
glm::vec3 reflect(const glm::vec3& incident, const glm::vec3& normal);
bool solveQuadratic(const float& a, const float& b, const float&c, float& x1, float& x2);
bool planeRayIntersection(const glm::vec3 & pointInPlane, const glm::vec3 & planeNormal, const Ray & r, glm::vec3 & hitPosition, glm::vec3 & hitNormal);
void barycentric(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float &u, float &v, float &w);
bool triangleRayIntersection(const glm::vec3 & t1, const glm::vec3 & t2, const glm::vec3 & t3,
                             const Ray & r, glm::vec3 & hitPosition, glm::vec3 & hitNormal, glm::vec3 & barycentricCoords);

std::ostream& operator << ( std::ostream& out, const glm::vec3& v);
std::ostream& operator << ( std::ostream& out, const glm::mat4& m);

#endif //UTILS_HPP
