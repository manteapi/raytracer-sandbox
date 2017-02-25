#include "./../include/raytracer-sandbox/ray.hpp"
#include <iostream>

Ray::~Ray(){}

Ray::Ray(const glm::vec3 &origin, const glm::vec3& direction)
{
    m_origin = origin;
    m_direction = direction;
    m_invDirection = glm::vec3(1.0/direction[0], 1.0/direction[1], 1.0/direction[2]);
    m_sign = {{ m_invDirection[0]<0, m_invDirection[1]<0, m_invDirection[2]<0 }};
    m_direction = glm::normalize(m_direction);
}

const glm::vec3& Ray::direction() const{ return m_direction; }

const glm::vec3& Ray::origin() const{ return m_origin; }

const glm::vec3& Ray::invDirection() const{ return m_invDirection; }

const std::array<int,3>& Ray::sign() const{ return m_sign; }

std::ostream& operator << ( std::ostream& out, const Ray& ray)
{
    glm::vec3 dir = ray.direction();
    glm::vec3 o = ray.origin();
    out << "Direction : " << dir[0] << ", " << dir[1] << ", " << dir[2] << std::endl;
    out << "Origin : " << o[0] << ", " << o[1] << ", " << o[2] << std::endl;
    return out;
}
