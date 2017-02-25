#include "./../include/raytracer-sandbox/sphere.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

Sphere::~Sphere(){}

Sphere::Sphere(const glm::vec3& position, const float& radius, const MaterialPtr &material)
{
    m_position = position;
    m_radius = radius;
    m_material = material;
    glm::vec3 minBB = m_position-glm::vec3(m_radius,m_radius,m_radius);
    glm::vec3 maxBB = m_position+glm::vec3(m_radius,m_radius,m_radius);
    this->bbox() = Box(minBB, maxBB);
}

glm::vec3& Sphere::position(){return m_position;}

const glm::vec3& Sphere::position() const {return m_position;}

float& Sphere::radius(){return m_radius;}

const float& Sphere::radius() const {return m_radius;}

bool Sphere::Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal)
{
    float a = glm::length2(r.direction());
    float b = 2.0*glm::dot(r.direction(), r.origin()-this->position());
    float c = glm::length2(r.origin()-this->position())-this->radius()*this->radius();
    float t0=-1, t1=-1;
    bool intersect = solveQuadratic(a,b,c,t0,t1);
    if(intersect)
    {
        if(t0<0 && t1<0) return false;
        float minT = t0;
        if(t0<t1 && t0>0) minT = t0;
        if(t0>t1 && t1>0) minT = t1;
        if(t0>0 || t1>0)
        {
            hitPosition = r.origin() + minT*r.direction();
            hitNormal = glm::normalize(hitPosition-this->position());
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

std::ostream& operator << ( std::ostream& out, const Sphere& o)
{
    glm::vec3 position = o.position();
    out << "Radius : " << o.radius() << std::endl;
    out << "Position : " << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
    return out;
}
