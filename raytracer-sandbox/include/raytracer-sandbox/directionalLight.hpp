#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "light.hpp"

/**
 * @brief A directional light.
 *
 * This class represents a directional light which means a light
 * whose source is so far away from the camera that the light rays
 * can be considered close to parallel from each other regardless of
 * the position of the objects or the camera. A good example is the
 * sun.
 */
class DirectionalLight : public Light
{
public:

    typedef std::shared_ptr<DirectionalLight> DirectionalLightPtr; /*!< Smart pointer to a directional light */

    /**
     * @brief Destructor
     */
    ~DirectionalLight();

    /**
     * @brief Default constructor
     */
    DirectionalLight() = default;

    /**
     * @brief Copy constructor
     */
    DirectionalLight(const DirectionalLight& directionalLight) = default;

    /**
    * @brief Specific constructor
    *
    * Construct a directional light.
    *
    * @param direction The direction of the light.
    * @param ambient The ambient intensity of the light.
    * @param diffuse The diffuse intensity of the light.
    * @param specular The specular intensity of the light.
    */
    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    /**
     * @brief Access to the direction of the light.
     *
     * @return A const reference to m_direction.
     */
    const glm::vec3& direction() const;

    /**
     * @brief Set the direction of the light.
     *
     * Set the value of m_direction.
     * @param direction The new direction of the light.
     */
    void setDirection(const glm::vec3 &direction);

    /**
     * @brief Access to the ambient intensity of the light.
     *
     * @return A const reference to m_ambient.
     */
    const glm::vec3 &ambient() const;

    /**
     * @brief Set the ambient intensity of the light.
     *
     * Set the value of m_ambient.
     * @param ambient The new ambient intensity of the light.
     */
    void setAmbient(const glm::vec3 &ambient);

    /**
     * @brief Access to the diffuse intensity of the light.
     *
     * @return A const reference to m_diffuse.
     */
    const glm::vec3& diffuse() const;

    /**
     * @brief Set the diffuse intensity of the light.
     *
     * Set the value of m_diffuse.
     * @param diffuse The new diffuse intensity of the light.
     */
    void setDiffuse(const glm::vec3 &diffuse);

    /**
     * @brief Access to the specular intensity of the light.
     *
     * @return A const reference to m_specular.
     */
    const glm::vec3 &specular() const;

    /**
     * @brief Set the specular intensity of the light.
     *
     * Set the value of m_specular.
     * @param specular The new specular intensity of the light.
     */
    void setSpecular(const glm::vec3 &specular);

    virtual glm::vec3 phongIllumination( const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                      const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const;
    virtual glm::vec3 lightDirectionFrom(const glm::vec3& position) const;

private:
    glm::vec3 m_direction;  /*!< The direction of the light. */
    glm::vec3 m_ambient;    /*!< Intensity of the light with respect to the object ambient components. */
    glm::vec3 m_diffuse;    /*!< Intensity of the light with respect to the object diffuse components. */
    glm::vec3 m_specular;   /*!< Intensity of the light with respect to the object specular components. */
};

typedef std::shared_ptr<DirectionalLight> DirectionalLightPtr; /*!< Smart pointer to a directional light */

#endif //DIRECTIONALLIGHT_HPP
