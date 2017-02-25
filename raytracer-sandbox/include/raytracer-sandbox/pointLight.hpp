#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "light.hpp"

/**
 * @brief A point light.
 *
 * This class represents a point light which means a light
 * with a given position somewhere in a world that illuminates
 * in all directions. Good examples of point light are torches.
 */
class PointLight : public Light
{
public:
    typedef std::shared_ptr<PointLight> PointLightPtr; /*!< Smart pointer to a point light */

    /**
     * @brief Destructor
     */
    ~PointLight();

    /**
     * @brief Default constructor
     */
    PointLight() = default;

    /**
     * @brief Default constructor
     */
    PointLight(const PointLight& pointLight) = default;

    /**
     * @brief Specific constructor
     *
     * Construct a point light.
     *
     * @param position The position of the light.
     * @param ambient The ambient intensity of the light.
     * @param diffuse The diffuse intensity of the light.
     * @param specular The specular intensity of the light.
     * @param constant The coefficient of constant attenuation of the light.
     * @param linear The coefficient of linear attenuation of the light with respect to the distance to the light.
     * @param quadratic The coefficient of quadratic attenuation of the light with respect to the distance to the light.
     */
    PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
               const float& constant, const float& linear, const float& quadratic);

    /**
     * @brief Access to the position of the light.
     *
     * @return A const reference to m_position.
     */
    const glm::vec3 &position() const;

    /**
     * @brief Set the position of the light.
     *
     * Set the value of m_position.
     * @param position The new position of the light.
     */
    void setPosition(const glm::vec3 &position);

    /**
     * @brief Access to the ambient intensity of the light.
     *
     * @return A const reference to m_ambient.
     */
    const glm::vec3& ambient() const;

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
    const glm::vec3 &diffuse() const;

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
    const glm::vec3& specular() const;

    /**
     * @brief Set the specular intensity of the light.
     *
     * Set the value of m_specular.
     * @param specular The new specular intensity of the light.
     */
    void setSpecular(const glm::vec3 &specular);

    /**
     * @brief Access to the coefficient of constant attenuation of the light.
     *
     * @return A const reference to m_constant.
     */
    const float &constant() const;

    /**
     * @brief Set the coefficient of constant attenuation of the light.
     *
     * Set the value of m_constant.
     * @param constant The new coefficient of constant attenuation of the light.
     */
    void setConstant(float constant);

    /**
     * @brief Access to the coefficient of linear attenuation of the light.
     *
     * @return A const reference to m_linear.
     */
    const float& linear() const;

    /**
     * @brief Set the coefficient of linear attenuation of the light.
     *
     * Set the value of m_linear.
     * @param linear The new coefficient of linear attenuation of the light.
     */
    void setLinear(float linear);

    /**
     * @brief Access to the coefficient of quadratic attenuation of the light.
     *
     * @return A const reference to m_quadratic.
     */
    const float &quadratic() const;

    /**
     * @brief Set the coefficient of quadratic attenuation of the light.
     *
     * Set the value of m_quadratic.
     * @param quadratic The new coefficient of quadratic attenuation of the light.
     */
    void setQuadratic(float quadratic);

    virtual glm::vec3 phongIllumination( const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                      const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const;
    virtual glm::vec3 lightDirectionFrom(const glm::vec3& position) const;

private:
    glm::vec3 m_position; /*!< The position of the light. */

    glm::vec3 m_ambient;    /*!< Intensity of the light with respect to the object ambient components. */
    glm::vec3 m_diffuse;    /*!< Intensity of the light with respect to the object diffuse components. */
    glm::vec3 m_specular;   /*!< Intensity of the light with respect to the object specular components. */

    float m_constant;       /*!< Coefficient of constant attenuation of the light. */
    float m_linear;         /*!< Coefficient of linear attenuation of the light with respect to the distance to the light position. */
    float m_quadratic;      /*!< Coefficient of quadratic attenuation of the light with respect to the distance to the light position. */
};

typedef std::shared_ptr<PointLight> PointLightPtr; /*!< Smart pointer to a point light */

#endif // POINTLIGHT_HPP
