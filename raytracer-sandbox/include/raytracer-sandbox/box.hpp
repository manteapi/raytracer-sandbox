#ifndef BOX_HPP
#define BOX_HPP

#include <array>
#include <glm/glm.hpp>
#include "ray.hpp"

class Box
{
public:
    ~Box() = default;
    Box() = default;
    Box(const Box& box) = default;
    Box(const glm::vec3& minBounds, const glm::vec3& maxBounds);
    std::array< glm::vec3, 2 >& bounds();
    const std::array< glm::vec3, 2 >& bounds() const;
    glm::vec3& maxExtent();
    const glm::vec3& maxExtent() const;
    glm::vec3& minExtent();
    const glm::vec3& minExtent() const;
private:
    std::array< glm::vec3, 2 > m_bounds;
};

bool Intersect(const Ray &r, const Box& box, std::array<float, 2> &t);

#endif // BOX_HPP
