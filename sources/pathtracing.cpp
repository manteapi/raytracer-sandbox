#include "./../headers/pathtracing.hpp"
#include <iostream>

bool pathTrace(const Ray& ray, const std::vector<ObjectPtr>& objects, ObjectPtr& closestHitObject, QVector3D& closestHitPosition, QVector3D& closestHitNormal)
{
    bool narrowIntersection = false;
    closestHitObject = nullptr;
    QVector3D hitPosition, hitNormal;
    float minDistance = std::numeric_limits<float>::max();
    for(const ObjectPtr& o : objects)
    {
        //Broad phase
        std::array<float, 2> tValue = {{0,0}};
        bool broadIntersection = true;
        broadIntersection = Intersect(ray, o->bbox(), tValue) && (tValue[0] >= 0 || tValue[1] >=0);

        //Narrow phase
        if( broadIntersection )
        {
            if(o->Intersect(ray, hitPosition, hitNormal))
            {
                narrowIntersection = true;
                float distance = (hitPosition-ray.origin()).length();
                if(distance < minDistance)
                {
                    closestHitObject = o;
                    closestHitPosition = hitPosition;
                    closestHitNormal = hitNormal.normalized();
                    minDistance = distance;
                }
            }
        }
    }
    return narrowIntersection;
}

QVector3D castRay(const Ray& ray, const std::vector<LightPtr> &lights, const std::vector<ObjectPtr> &objects,
                  const QVector3D& backgroundColor, const QVector3D& shadowColor, const float& bias, const int& maxDepth, int depth)
{
    if(depth>maxDepth) return backgroundColor;

    ObjectPtr closestHitObject = nullptr;
    QVector3D closestHitPosition, closestHitNormal;

    //Check intersection between the ray and the scene
    pathTrace(ray, objects, closestHitObject, closestHitPosition, closestHitNormal);

    //Compute illumination
    QVector3D color(0,0,0);
    if(closestHitObject != nullptr)
    {
        switch (closestHitObject->material()->type())
        {
        case MaterialType::GLOSSY:
        {
            QVector3D direction = (closestHitPosition-ray.origin()).normalized();
            QVector3D reflectDirection = reflect(direction, closestHitNormal).normalized();
            bool outside = QVector3D::dotProduct(direction, closestHitNormal) < 0;
            QVector3D biasVector = QVector3D(bias,bias,bias) * closestHitNormal;
            QVector3D reflectionRayOrig = outside ? closestHitPosition + biasVector : closestHitPosition - biasVector;
            Ray reflectionRay(reflectionRayOrig, reflectDirection);
            color += castRay(reflectionRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth+1);
            break;
        }
        case MaterialType::FRESNEL:
        {
            QVector3D refractionColor(0,0,0), reflectionColor(0,0,0);
            FresnelMaterialPtr material = std::static_pointer_cast<FresnelMaterial>(closestHitObject->material());
            float kr=0.0, kt=0.0;
            QVector3D direction = (closestHitPosition-ray.origin()).normalized();
            fresnel(direction, closestHitNormal, material->ior(), kr, kt);
            bool outside = QVector3D::dotProduct(direction, closestHitNormal) < 0;
            QVector3D biasVector = QVector3D(bias,bias,bias) * closestHitNormal;            
            // compute refraction if it is not a case of total internal reflection
            if (kr < 1)
            {
                QVector3D refractionDirection = refract(direction, closestHitNormal, material->ior()).normalized();
                QVector3D refractionRayOrig = outside ? closestHitPosition - biasVector : closestHitPosition + biasVector;
                Ray refractionRay(refractionRayOrig, refractionDirection);                
                refractionColor = castRay(refractionRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth+1);
            }
            QVector3D reflectionDirection = reflect(direction, closestHitNormal).normalized();
            QVector3D reflectionRayOrig = outside ? closestHitPosition + biasVector : closestHitPosition - biasVector;
            Ray reflectionRay(reflectionRayOrig, reflectionDirection);
            reflectionColor = castRay(reflectionRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth+1);
            color += reflectionColor * kr + refractionColor * kt;
            break;
        }
        case MaterialType::PHONG:
        {
            for(const LightPtr& light : lights)
            {
                QVector3D biasVector = QVector3D(bias,bias,bias) * closestHitNormal;
                QVector3D shadowRayOrig = closestHitPosition + biasVector;
                Ray shadowRay(shadowRayOrig, -light->lightDirectionFrom(closestHitPosition));
                ObjectPtr shadowClosestHitObject = nullptr;
                QVector3D shadowClosestHitPosition, shadowClosestHitNormal;
                pathTrace(shadowRay, objects, shadowClosestHitObject, shadowClosestHitPosition, shadowClosestHitNormal);
                bool isInShadow = (shadowClosestHitObject!=nullptr && shadowClosestHitObject.get() != closestHitObject.get()) ? true : false;
                bool materialWithShadow = false;
                if(isInShadow)
                {
                    materialWithShadow = shadowClosestHitObject->material()->type() != MaterialType::FRESNEL;
                    //materialWithShadow = materialWithShadow && shadowClosestHitObject->material()->type() != MaterialType::GLOSSY;
                }
                if(isInShadow && materialWithShadow)
                {
                    color = shadowColor;
                }
                else
                {

                    PhongMaterialPtr material = std::static_pointer_cast<PhongMaterial>(closestHitObject->material());
                    color += light->phongIllumination(ray.origin(), closestHitPosition,closestHitNormal, material);
                }
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    else
    {
        color = backgroundColor;
    }
    return color;
}
