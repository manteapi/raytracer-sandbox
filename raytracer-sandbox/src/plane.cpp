#include "./../include/raytracer-sandbox/plane.hpp"

using namespace std;

Plane::Plane(const glm::vec3& normal,
             const glm::vec3& point , const MaterialPtr &material)
    : m_n{ glm::normalize(normal) },
      m_d{ glm::dot( normal, point ) }
{
    this->material() = material;

    glm::vec3 minBB( -numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max() );
    glm::vec3 maxBB( numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max() );
    this->bbox() = Box(minBB, maxBB);
}

Plane::Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const MaterialPtr &material)
{
    m_n = glm::normalize(glm::cross(b-a, c-a));
    m_d = glm::dot(m_n, a);
    m_material = material;

    glm::vec3 minBB( -numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max() );
    glm::vec3 maxBB( numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max() );
    this->bbox() = Box(minBB, maxBB);
}

glm::vec3 Plane::projectOnPlane(const glm::vec3& p)
{
    glm::vec3 planePoint = m_d*m_n;
    glm::vec3 v = p-planePoint;
    float dist = glm::dot(v,m_n);
    glm::vec3 projection = p - dist*m_n;
    return projection;
}

bool Plane::Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal)
{
    float dotNormalRayDir = glm::dot(r.direction(), m_n);
    if ( abs(dotNormalRayDir) > numeric_limits<float>::epsilon())
    {
        float t = (m_d - glm::dot(r.origin(), m_n)) / glm::dot(r.direction(), m_n);
        hitPosition = r.origin() + t*r.direction();
        hitNormal = m_n;
        if(t>=0)
            return true;
    }
    return false;
}

void Plane::setDistanceToOrigin(const float& d)
{
    m_d = d;
}

const float& Plane::distanceToOrigin() const
{
    return m_d;
}

void Plane::setNormal(const glm::vec3& n)
{
    m_n = glm::normalize(n);
}

const glm::vec3& Plane::normal() const
{
    return m_n;
}
