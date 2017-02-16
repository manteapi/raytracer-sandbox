#include "./../headers/utils.hpp"

using namespace std;

float clamp(float n, float lower, float upper)
{
    return std::max(lower, std::min(n, upper));
}

void fresnel(const QVector3D &incident, const QVector3D &normal, const float &ior, float &kr, float & kt)
{
    float cosi = clamp(QVector3D::dotProduct(incident, normal), -1.0, 1.0);
    float etai = 1.0, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrt(std::max(0.0f, 1.0f - cosi * cosi));
    // Total internal reflection
    if (sint >= 1)
    {
        kr = 1;
    }
    else
    {
        float cost = sqrt(std::max(0.0f, 1 - sint * sint));
        cosi = std::abs(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2.0;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    kt = 1 - kr;
}

QVector3D refract(const QVector3D &incident, const QVector3D &normal, const float &ior)
{
    float cosi = clamp(QVector3D::dotProduct(incident, normal), -1.0, 1.0);
    float etai = 1.0, etat = ior;
    QVector3D n = normal;
    if (cosi < 0) { cosi = -cosi; } //Outside the surface
    else { std::swap(etai, etat); n= -normal ;} //Inside the surface
    float eta = etai / etat;
    float k = 1.0 - eta * eta * (1.0 - cosi * cosi);
    //if(k<0) then total reflection else refraction
    return (k < 0) ? QVector3D(0,0,0) : (eta * incident + (eta * cosi - sqrt(k)) * n);
}

QVector3D reflect(const QVector3D& incident, const QVector3D& normal)
{
    return incident-2.0f*QVector3D::dotProduct(incident, normal)*normal;
}

ostream& operator << ( ostream& out, const QVector3D& v)
{
    out << v[0] << ", " << v[1] << ", " << v[2] << endl;
    return out;
}

ostream& operator << ( ostream& out, const QMatrix4x4& m)
{
    out << m.row(0)[0] << " " << m.row(0)[1] << " " << m.row(0)[2] << " " << m.row(0)[3] << endl;
    out << m.row(1)[0] << " " << m.row(1)[1] << " " << m.row(1)[2] << " " << m.row(1)[3] << endl;
    out << m.row(2)[0] << " " << m.row(2)[1] << " " << m.row(2)[2] << " " << m.row(2)[3] << endl;
    out << m.row(3)[0] << " " << m.row(3)[1] << " " << m.row(3)[2] << " " << m.row(3)[3] << endl;
    return out;
}

bool solveQuadratic(const float& a, const float& b, const float&c, float& x1, float& x2)
{
    float delta = b*b-4*a*c;
    if(delta<0)
    {
        return false;
    }
    else if(delta==0)
    {
        x1 = -b/(2*a);
        x2 = x1;
    }
    else if(delta>0)
    {
        x1 = (-b+sqrt(delta))/(2*a);
        x2 = (-b-sqrt(delta))/(2*a);
    }
    return true;
}


bool planeRayIntersection(const QVector3D & pointInPlane, const QVector3D & planeNormal, const Ray & r, QVector3D & hitPosition, QVector3D & hitNormal)
{
    float distanceToOrigin = QVector3D::dotProduct(planeNormal, pointInPlane);
    float dotNormalRayDir = QVector3D::dotProduct(r.direction(), planeNormal);
    if ( std::abs(dotNormalRayDir) > std::numeric_limits<float>::epsilon())
    {
        float t = (distanceToOrigin - QVector3D::dotProduct(r.origin(), planeNormal)) / QVector3D::dotProduct(r.direction(), planeNormal);
        hitPosition = r.origin() + t*r.direction();
        hitNormal = planeNormal;
        if(t>=0) return true;
    }
    return false;
}

void barycentric(const QVector3D &p, const QVector3D &a, const QVector3D &b, const QVector3D &c, float &u, float &v, float &w)
{
    QVector3D v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = QVector3D::dotProduct(v0, v0);
    float d01 = QVector3D::dotProduct(v0, v1);
    float d11 = QVector3D::dotProduct(v1, v1);
    float d20 = QVector3D::dotProduct(v2, v0);
    float d21 = QVector3D::dotProduct(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}

bool triangleRayIntersection(const QVector3D & t1, const QVector3D & t2, const QVector3D & t3, const Ray & r, QVector3D &hitPosition, QVector3D &hitNormal)
{
    //Compute the normal of the plane containing the triangle
    QVector3D planeNormal = QVector3D::crossProduct(t2-t1, t3-t1);

    //Check the intersection of the ray and the plane
    QVector3D pHitPosition, pHitNormal;
    bool intersect = planeRayIntersection(t1, planeNormal, r, pHitPosition, pHitNormal);
    if(!intersect) return false;
    else
    {
        float u, v, w;
        barycentric(pHitPosition, t1, t2, t3, u, v, w);
        if(u>=0 && u<=1 && v>=0 && v<=1 && w>=0 && w<=1)
        {
            hitPosition = pHitPosition;
            hitNormal = pHitNormal;
            return true;
        }
        else
            return false;
    }
}

QColor toColor(const QVector3D& c)
{
    return QColor(255*clamp(c[0],0,1), 255*clamp(c[1],0,1), 255*clamp(c[2],0,1));
}
