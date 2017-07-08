#include "./../include/raytracer-sandbox/box.hpp"

Box::Box(const glm::vec3& minBounds, const glm::vec3& maxBounds)
{
    m_bounds[0] = minBounds;
    m_bounds[1] = maxBounds;
}

std::array< glm::vec3, 2 >& Box::bounds()
{
    return m_bounds;
}

const std::array< glm::vec3, 2 >& Box::bounds() const
{
    return m_bounds;
}

glm::vec3& Box::maxBound()
{
    return m_bounds[1];
}

const glm::vec3& Box::maxBound() const
{
    return m_bounds[1];
}

glm::vec3& Box::minBound()
{
    return m_bounds[0];
}

const glm::vec3& Box::minBound() const
{
    return m_bounds[0];
}

bool Intersect(const Ray &r, const Box& box, std::array<float,2>& t)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    const std::array<glm::vec3,2>& bbounds = box.bounds();
    const std::array<int,3>& rSign = r.sign();
    const glm::vec3& rInvDir = r.invDirection();
    const glm::vec3& rOrig = r.origin();

    tmin = (bbounds[rSign[0]][0] - rOrig[0]) * rInvDir[0];
    tmax = (bbounds[1-rSign[0]][0] - rOrig[0]) * rInvDir[0];

    tymin = (bbounds[rSign[1]][1] - rOrig[1]) * rInvDir[1];
    tymax = (bbounds[1-rSign[1]][1] - rOrig[1]) * rInvDir[1];

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bbounds[rSign[2]][2] - rOrig[2]) * rInvDir[2];
    tzmax = (bbounds[1-rSign[2]][2] - rOrig[2]) * rInvDir[2];

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    t[0] = std::min(tmin,tmax);
    t[1] = std::max(tmin,tmax);

    return true;
}
