#include "./../include/raytracer-sandbox/object.hpp"
#include "./../include/raytracer-sandbox/io.hpp"
#include <iostream>
#include <assert.h>
#include <random>
#include <glm/gtx/norm.hpp>

using namespace std;

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

glm::vec3& Box::maxExtent()
{
    return m_bounds[1];
}

const glm::vec3& Box::maxExtent() const
{
    return m_bounds[1];
}

glm::vec3& Box::minExtent()
{
    return m_bounds[0];
}

const glm::vec3& Box::minExtent() const
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

Object::~Object(){}

MaterialPtr &Object::material()
{
    return m_material;
}

const MaterialPtr &Object::material() const
{
    return m_material;
}

Box& Object::bbox()
{
    return m_bbox;
}

const Box& Object::bbox() const
{
    return m_bbox;
}

TMesh::~TMesh(){}

TMesh::TMesh(const std::string& filename, const MaterialPtr& material)
{
    this->material() = material;

    read_obj(filename, m_positions, m_indices, m_normals, m_texCoords);

    glm::vec3 minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    glm::vec3 maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
    for(size_t i=0; i<m_positions.size(); ++i)
    {
        for(size_t j=0; j<3; ++j)
        {
            minBB[j] = m_positions[i][j]<minBB[j] ? m_positions[i][j] : minBB[j];
            maxBB[j] = m_positions[i][j]>maxBB[j] ? m_positions[i][j] : maxBB[j];
        }
    }
    this->bbox() = Box(minBB, maxBB);
}

bool TMesh::Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal)
{
    glm::vec3 barycentricCoords;
    for(size_t i=0; i<m_indices.size()/3; i++)
    {
        if(triangleRayIntersection(m_positions[ m_indices[3*i] ], m_positions[ m_indices[3*i+1] ], m_positions[ m_indices[3*i+2] ], r, hitPosition, hitNormal, barycentricCoords))
        {
            hitNormal = barycentricCoords[0]*m_normals[m_indices[3*i]] + barycentricCoords[1]*m_normals[m_indices[3*i+1]] + barycentricCoords[2]*m_normals[m_indices[3*i+2]];
            hitNormal = glm::normalize(hitNormal);
            return true;
        }
    }
    return false;
}

Plane::Plane(const glm::vec3& normal,
             const glm::vec3& point , const MaterialPtr &material)
    : m_n{ glm::normalize(normal) },
      m_d{ glm::dot( normal, point ) }
{
    this->material() = material;

    glm::vec3 minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    glm::vec3 maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
    this->bbox() = Box(minBB, maxBB);
}

Plane::Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const MaterialPtr &material)
{
    m_n = glm::normalize(glm::cross(b-a, c-a));
    m_d = glm::dot(m_n, a);
    m_material = material;

    glm::vec3 minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    glm::vec3 maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
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
    if ( std::abs(dotNormalRayDir) > std::numeric_limits<float>::epsilon())
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
