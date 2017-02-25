#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "light.hpp"

/**
 * @brief A spot light.
 *
 * This class represents a spot light which means a light
 * whose source is located somewhere in the environment and
 * instead of shooting light rays in all directions,
 * only shoots them in a specific direction.
 * As a result, only the objects within a certain radius
 * of the spotlight's direction are lit and everything else stays dark.
 * A good example of a spotlight would be a street lamp.
 */
class SpotLight : public Light
{
public:
    typedef std::shared_ptr<SpotLight> SpotLightPtr; /*!< Smart pointer to a spot light */

    /**
     * @brief Destructor
     */
    ~SpotLight();

    /**
     * @brief Default constructor
     */
    SpotLight() = default;

    /**
     * @brief Copy constructor
     */
    SpotLight(const SpotLight& spotLight) = default;

    /**
    * @brief Specific constructor
    *
    * Construct a spot light.
    *
    * @param position The position of the light.
    * @param spotDirection The direction of the light.
    * @param ambient The ambient intensity of the light.
    * @param diffuse The diffuse intensity of the light.
    * @param specular The specular intensity of the light.
    * @param constant The coefficient of constant attenuation of the light.
    * @param linear The coefficient of linear attenuation of the light with respect to the distance to the light.
    * @param quadratic The coefficient of quadratic attenuation of the light with respect to the distance to the light.
    * @param innerCutOff The cosinus of the inner cut off angle of the spot.
    * @param outerCutOff The cosinus of the outer cut off angle of the spot.
    */
    SpotLight(const glm::vec3& position, const glm::vec3& spotDirection,
              const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
              const float& constant, const float& linear, const float& quadratic,
              const float& innerCutOff, const float& outerCutOff);

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
     * @brief Access to the direction of the spot.
     *
     * @return A const reference to m_direction.
     */
    const glm::vec3 &spotDirection() const;

    /**
     * @brief Set the direction of the spot.
     *
     * Set the value of m_direction.
     * @param spotDirection The new direction of the light.
     */
    void setSpotDirection(const glm::vec3 &spotDirection);

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

    /**
     * @brief Access to the cosinus of the inner cut off angle of the spot.
     *
     * @return A const reference to m_innerCutOff.
     */
    const float& innerCutOff() const;

    /**
     * @brief Set the cosinus of the inner cut off angle of the spot.
     *
     * Set the value of m_innerCutOff.
     * @param innerCutOff The new cosinus of the inner cut off angle of the spot.
     */
    void setInnerCutOff(float innerCutOff);

    /**
     * @brief Access to the cosinus of the outer cut off angle of the spot.
     *
     * @return A const reference to m_outerCutOff.
     */
    const float &outerCutOff() const;

    /**
     * @brief Set the cosinus of the outer cut off angle of the spot.
     *
     * Set the value of m_outerCutOff.
     * @param outerCutOff The new cosinus of the outer cut off angle of the spot.
     */
    void setOuterCutOff(float outerCutOff);

    virtual glm::vec3 phongIllumination(const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                        const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const;
    virtual glm::vec3 lightDirectionFrom(const glm::vec3& position) const;

private:
    glm::vec3 m_position; /*!< The position of the light. */
    glm::vec3 m_spotDirection; /*!< The direction of the spot. */

    glm::vec3 m_ambient;    /*!< Intensity of the light with respect to the object ambient components. */
    glm::vec3 m_diffuse;    /*!< Intensity of the light with respect to the object diffuse components. */
    glm::vec3 m_specular;   /*!< Intensity of the light with respect to the object specular components. */

    float m_constant;       /*!< Coefficient of constant attenuation of the light. */
    float m_linear;         /*!< Coefficient of linear attenuation of the light with respect to the distance to the light position. */
    float m_quadratic;      /*!< Coefficient of quadratic attenuation of the light with respect to the distance to the light position. */

    float m_innerCutOff;    /*!< The cosinus of the inner cutoff angle that specifies the spotlight's inner radius. Everything inside this angle is fully lit by the spotlight. */
    float m_outerCutOff;    /*!< The cosinus of the outer cutoff angle that specifies the spotlight's outer radius. Everything outside this angle is not lit by the spotlight. */
};

typedef std::shared_ptr<SpotLight> SpotLightPtr; /*!< Smart pointer to a spot light */

#endif // SPOTLIGHT_HPP
