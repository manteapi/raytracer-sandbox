#include "./../include/raytracer-sandbox/pathtracing.hpp"
#include <iostream>

bool pathTrace(const Ray& ray, const std::vector<ObjectPtr>& objects, ObjectPtr& closestHitObject, glm::vec3& closestHitPosition, glm::vec3& closestHitNormal)
{
    bool narrowIntersection = false;
    closestHitObject = nullptr;
    glm::vec3 hitPosition, hitNormal;
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
                    closestHitNormal = glm::normalize(hitNormal);
                    minDistance = distance;
                }
            }
        }
    }
    return narrowIntersection;
}

glm::vec3 castRay(const Ray& ray, const std::vector<LightPtr> &lights, const std::vector<ObjectPtr> &objects,
                  const glm::vec3& backgroundColor, const glm::vec3& shadowColor, const float& bias, const int& maxDepth, int depth)
{
    if(depth>maxDepth) return backgroundColor;

    ObjectPtr closestHitObject = nullptr;
    glm::vec3 closestHitPosition, closestHitNormal;

    //Check intersection between the ray and the scene
    pathTrace(ray, objects, closestHitObject, closestHitPosition, closestHitNormal);

    //Compute illumination
    glm::vec3 color(0,0,0);
    if(closestHitObject != nullptr)
    {
        switch (closestHitObject->material()->type())
        {
        case MaterialType::GLOSSY:
        {
            glm::vec3 direction = glm::normalize(closestHitPosition-ray.origin());
            glm::vec3 reflectDirection = glm::normalize(reflect(direction, closestHitNormal));
            bool outside = glm::dot(direction, closestHitNormal) < 0;
            glm::vec3 biasVector = glm::vec3(bias,bias,bias) * closestHitNormal;
            glm::vec3 reflectionRayOrig = outside ? closestHitPosition + biasVector : closestHitPosition - biasVector;
            Ray reflectionRay(reflectionRayOrig, reflectDirection);
            color += castRay(reflectionRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth+1);
            break;
        }
        case MaterialType::FRESNEL:
        {
            glm::vec3 refractionColor(0,0,0), reflectionColor(0,0,0);
            FresnelMaterialPtr material = std::static_pointer_cast<FresnelMaterial>(closestHitObject->material());
            float kr=0.0, kt=0.0;
            glm::vec3 direction = glm::normalize(closestHitPosition-ray.origin());
            fresnel(direction, closestHitNormal, material->ior(), kr, kt);
            bool outside = glm::dot(direction, closestHitNormal) < 0;
            glm::vec3 biasVector = glm::vec3(bias,bias,bias) * closestHitNormal;            
            // compute refraction if it is not a case of total internal reflection
            if (kr < 1)
            {
                glm::vec3 refractionDirection = glm::normalize(refract(direction, closestHitNormal, material->ior()));
                glm::vec3 refractionRayOrig = outside ? closestHitPosition - biasVector : closestHitPosition + biasVector;
                Ray refractionRay(refractionRayOrig, refractionDirection);                
                refractionColor = castRay(refractionRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth+1);
            }
            glm::vec3 reflectionDirection = glm::normalize(reflect(direction, closestHitNormal));
            glm::vec3 reflectionRayOrig = outside ? closestHitPosition + biasVector : closestHitPosition - biasVector;
            Ray reflectionRay(reflectionRayOrig, reflectionDirection);
            reflectionColor = castRay(reflectionRay, lights, objects, backgroundColor, shadowColor, bias, maxDepth, depth+1);
            color += reflectionColor * kr + refractionColor * kt;
            break;
        }
        case MaterialType::PHONG:
        {
            for(const LightPtr& light : lights)
            {
                glm::vec3 biasVector = glm::vec3(bias,bias,bias) * closestHitNormal;
                glm::vec3 shadowRayOrig = closestHitPosition + biasVector;
                Ray shadowRay(shadowRayOrig, -light->lightDirectionFrom(closestHitPosition));
                ObjectPtr shadowClosestHitObject = nullptr;
                glm::vec3 shadowClosestHitPosition, shadowClosestHitNormal;
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
