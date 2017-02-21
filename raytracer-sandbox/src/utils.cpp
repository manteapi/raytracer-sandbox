#include "./../include/raytracer-sandbox/utils.hpp"
#include <iostream>

using namespace std;

float clamp(float n, float lower, float upper)
{
    return std::max(lower, std::min(n, upper));
}

void fresnel(const glm::vec3 &incident, const glm::vec3 &normal, const float &ior, float &kr, float & kt)
{
    float cosi = clamp(glm::dot(incident, normal), -1.0, 1.0);
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

glm::vec3 refract(const glm::vec3 &incident, const glm::vec3 &normal, const float &ior)
{
    float cosi = clamp(glm::dot(incident, normal), -1.0, 1.0);
    float etai = 1.0, etat = ior;
    glm::vec3 n = normal;
    if (cosi < 0) { cosi = -cosi; } //Outside the surface
    else { std::swap(etai, etat); n= -normal ;} //Inside the surface
    float eta = etai / etat;
    float k = 1.0 - eta * eta * (1.0 - cosi * cosi);
    //if(k<0) then total reflection else refraction
    return (k < 0) ? glm::vec3(0,0,0) : (eta * incident + (eta * cosi - sqrt(k)) * n);
}

glm::vec3 reflect(const glm::vec3& incident, const glm::vec3& normal)
{
    return incident-2.0f*glm::dot(incident, normal)*normal;
}

ostream& operator << ( ostream& out, const glm::vec3& v)
{
    out << v[0] << ", " << v[1] << ", " << v[2] << endl;
    return out;
}

ostream& operator << ( ostream& out, const glm::mat4& m)
{
    out << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << endl;
    out << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << endl;
    out << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << endl;
    out << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << endl;
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


bool planeRayIntersection(const glm::vec3 & pointInPlane, const glm::vec3 & planeNormal, const Ray & r, glm::vec3 & hitPosition, glm::vec3 & hitNormal)
{
    float distanceToOrigin = glm::dot(planeNormal, pointInPlane);
    float dotNormalRayDir = glm::dot(r.direction(), planeNormal);
    if ( std::abs(dotNormalRayDir) > std::numeric_limits<float>::epsilon())
    {
        float t = (distanceToOrigin - glm::dot(r.origin(), planeNormal)) / glm::dot(r.direction(), planeNormal);
        hitPosition = r.origin() + t*r.direction();
        hitNormal = planeNormal;
        if(t>=0) return true;
    }
    return false;
}

void barycentric(const glm::vec3 &p, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, float &u, float &v, float &w)
{
    glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}

bool triangleRayIntersection(const glm::vec3 & t1, const glm::vec3 & t2, const glm::vec3 & t3, const Ray & r, glm::vec3 &hitPosition, glm::vec3 &hitNormal, glm::vec3 &barycentricCoords)
{
    //Compute the normal of the plane containing the triangle
    glm::vec3 planeNormal = glm::cross(t2-t1, t3-t1);

    //Check the intersection of the ray and the plane
    glm::vec3 pHitPosition, pHitNormal;
    bool intersect = planeRayIntersection(t1, planeNormal, r, pHitPosition, pHitNormal);
    if(!intersect) return false;
    else
    {
        float u, v, w;
        barycentric(pHitPosition, t1, t2, t3, u, v, w);
        if(u>=0 && u<=1 && v>=0 && v<=1 && w>=0 && w<=1)
        {
            hitPosition = pHitPosition;
            barycentricCoords = glm::vec3(u,v,w);
            hitNormal = pHitNormal;
            return true;
        }
        else
            return false;
    }
}
