#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"
#include <glm/glm.hpp>

class Sphere : public Object
{
public:
    ~Sphere();
    Sphere() = default;
    Sphere(const Sphere& s) = default;
    Sphere(const glm::vec3& position, const float& radius, const MaterialPtr& material);
    const glm::vec3& position() const;
    const float& radius() const;
    virtual bool Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal) const;
private:
    float m_radius;
    glm::vec3 m_position;
};

std::ostream& operator << ( std::ostream& out, const Sphere& o);

typedef std::shared_ptr<Sphere> SpherePtr;

#endif // SPHERE_HPP
