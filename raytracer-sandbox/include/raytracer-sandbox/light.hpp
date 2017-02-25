#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <memory>
#include <glm/glm.hpp>
#include "material.hpp"

/**
 * @brief Base class for light object.
 */
class Light
{
public:
    /**
     * @brief Destructor.
     */
    virtual ~Light();

    /**
     * @brief Compute the color of the surface position looked from a given viewpoint using the phong model.
     *
     * @return The color diffused to the eye position when the surface is lighted using phong model.
     * @param eyePosition The position from where the object is looked at.
     * @param surfacePosition The position of the surface looked by the eye.
     * @param surfaceNormal The normal of the surface at the position looked by the eye.
     * @param material The material describing the properties of the surface according to the phong model.
     */
    virtual glm::vec3 phongIllumination( const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                      const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const = 0;

    /**
     * @brief Return the direction of the light.
     *
     * Return the direction of the light from a given position.
     * For a directional light, the result will not depend on the position.
     * However for a point light or a spot light, it will.
     *
     * @return The direction of the light from a given position.
     * @param position The position from where we want to get the light direction.
     */
    virtual glm::vec3 lightDirectionFrom(const glm::vec3& position) const = 0;
};

typedef std::shared_ptr<Light> LightPtr;

#endif //LIGHT_HPP
