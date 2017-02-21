#include "./../headers/object.hpp"
#include "./../headers/io.hpp"
#include <iostream>
#include <assert.h>
#include <random>

using namespace std;

Box::Box(const QVector3D& minBounds, const QVector3D& maxBounds)
{
    m_bounds[0] = minBounds;
    m_bounds[1] = maxBounds;
}

std::array< QVector3D, 2 >& Box::bounds()
{
    return m_bounds;
}

const std::array< QVector3D, 2 >& Box::bounds() const
{
    return m_bounds;
}

QVector3D& Box::maxExtent()
{
    return m_bounds[1];
}

const QVector3D& Box::maxExtent() const
{
    return m_bounds[1];
}

QVector3D& Box::minExtent()
{
    return m_bounds[0];
}

const QVector3D& Box::minExtent() const
{
    return m_bounds[0];
}

bool Intersect(const Ray &r, const Box& box, std::array<float,2>& t)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    const std::array<QVector3D,2>& bbounds = box.bounds();
    const std::array<int,3>& rSign = r.sign();
    const QVector3D& rInvDir = r.invDirection();
    const QVector3D& rOrig = r.origin();

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

    QVector3D minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    QVector3D maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
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

bool TMesh::Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal)
{
    QVector3D barycentricCoords;
    for(size_t i=0; i<m_indices.size()/3; i++)
    {
        if(triangleRayIntersection(m_positions[ m_indices[3*i] ], m_positions[ m_indices[3*i+1] ], m_positions[ m_indices[3*i+2] ], r, hitPosition, hitNormal, barycentricCoords))
        {
            hitNormal = barycentricCoords[0]*m_normals[m_indices[3*i]] + barycentricCoords[1]*m_normals[m_indices[3*i+1]] + barycentricCoords[2]*m_normals[m_indices[3*i+2]];
            hitNormal.normalize();
            return true;
        }
    }
    return false;
}

Plane::Plane(const QVector3D& normal,
             const QVector3D& point , const MaterialPtr &material)
    : m_n{ normal.normalized() },
      m_d{ QVector3D::dotProduct( normal, point ) }
{
    this->material() = material;

    QVector3D minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    QVector3D maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
    this->bbox() = Box(minBB, maxBB);
}

Plane::Plane(const QVector3D& a, const QVector3D& b, const QVector3D& c, const MaterialPtr &material)
{
    m_n = QVector3D::crossProduct(b-a, c-a).normalized();
    m_d = QVector3D::dotProduct(m_n, a);
    m_material = material;

    QVector3D minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    QVector3D maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
    this->bbox() = Box(minBB, maxBB);
}

QVector3D Plane::projectOnPlane(const QVector3D& p)
{
    QVector3D planePoint = m_d*m_n;
    QVector3D v = p-planePoint;
    float dist = QVector3D::dotProduct(v,m_n);
    QVector3D projection = p - dist*m_n;
    return projection;
}

bool Plane::Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal)
{
    float dotNormalRayDir = QVector3D::dotProduct(r.direction(), m_n);
    if ( std::abs(dotNormalRayDir) > std::numeric_limits<float>::epsilon())
    {
        float t = (m_d - QVector3D::dotProduct(r.origin(), m_n)) / QVector3D::dotProduct(r.direction(), m_n);
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

void Plane::setNormal(const QVector3D& n)
{
    m_n = n.normalized();
}

const QVector3D& Plane::normal() const
{
    return m_n;
}


Sphere::~Sphere(){}

Sphere::Sphere(const QVector3D& position, const float& radius, const MaterialPtr &material)
{
    m_position = position;
    m_radius = radius;
    m_material = material;
    QVector3D minBB = m_position-QVector3D(m_radius,m_radius,m_radius);
    QVector3D maxBB = m_position+QVector3D(m_radius,m_radius,m_radius);
    this->bbox() = Box(minBB, maxBB);
}

QVector3D& Sphere::position(){return m_position;}

const QVector3D& Sphere::position() const {return m_position;}

float& Sphere::radius(){return m_radius;}

const float& Sphere::radius() const {return m_radius;}

bool Sphere::Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal)
{
    float a = r.direction().lengthSquared();
    float b = 2.0*QVector3D::dotProduct(r.direction(), r.origin()-this->position());
    float c = (r.origin()-this->position()).lengthSquared()-this->radius()*this->radius();
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
            hitNormal = (hitPosition-this->position()).normalized();
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
    QVector3D position = o.position();
    out << "Radius : " << o.radius() << std::endl;
    out << "Position : " << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
    return out;
}
