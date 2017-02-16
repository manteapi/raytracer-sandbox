#include "./../headers/ray.hpp"

Ray::~Ray(){}

Ray::Ray(const Ray& r)
{
    m_origin = r.origin();
    m_direction = r.direction();
    m_direction.normalize();
}

Ray::Ray(const QVector3D &origin, const QVector3D& direction)
{
    m_origin = origin;
    m_direction = direction;
    m_direction.normalize();
}

QVector3D& Ray::direction(){ return m_direction; }

const QVector3D& Ray::direction() const{ return m_direction; }

QVector3D& Ray::origin(){ return m_origin; }

const QVector3D& Ray::origin() const{ return m_origin; }

std::ostream& operator << ( std::ostream& out, const Ray& ray)
{
    QVector3D dir = ray.direction();
    QVector3D o = ray.origin();
    out << "Direction : " << dir[0] << ", " << dir[1] << ", " << dir[2] << std::endl;
    out << "Origin : " << o[0] << ", " << o[1] << ", " << o[2] << std::endl;
    return out;
}
