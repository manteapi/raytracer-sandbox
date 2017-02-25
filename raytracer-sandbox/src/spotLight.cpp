#include "./../include/raytracer-sandbox/spotLight.hpp"
#include "./../include/raytracer-sandbox/utils.hpp"

using namespace std;

SpotLight::~SpotLight()
{

}

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& spotDirection,
          const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
          const float& constant, const float& linear, const float& quadratic,
          const float& innerCutOff, const float& outerCutOff)
{
    m_position = position;
    m_spotDirection = glm::normalize(spotDirection);
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
    m_innerCutOff = innerCutOff;
    m_outerCutOff = outerCutOff;
}

const glm::vec3 &SpotLight::position() const
{
    return m_position;
}

void SpotLight::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

const glm::vec3 &SpotLight::spotDirection() const
{
    return m_spotDirection;
}

void SpotLight::setSpotDirection(const glm::vec3 &spotDirection)
{
    m_spotDirection = glm::normalize(spotDirection);
}

const glm::vec3 &SpotLight::ambient() const
{
    return m_ambient;
}

void SpotLight::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3& SpotLight::diffuse() const
{
    return m_diffuse;
}

void SpotLight::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3 &SpotLight::specular() const
{
    return m_specular;
}

void SpotLight::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

const float& SpotLight::constant() const
{
    return m_constant;
}

void SpotLight::setConstant(float constant)
{
    m_constant = constant;
}

const float &SpotLight::linear() const
{
    return m_linear;
}

void SpotLight::setLinear(float linear)
{
    m_linear = linear;
}

const float& SpotLight::quadratic() const
{
    return m_quadratic;
}

void SpotLight::setQuadratic(float quadratic)
{
    m_quadratic = quadratic;
}

const float &SpotLight::innerCutOff() const
{
    return m_innerCutOff;
}

void SpotLight::setInnerCutOff(float innerCutOff)
{
    m_innerCutOff = innerCutOff;
}

const float& SpotLight::outerCutOff() const
{
    return m_outerCutOff;
}

void SpotLight::setOuterCutOff(float outerCutOff)
{
    m_outerCutOff = outerCutOff;
}

glm::vec3 SpotLight::phongIllumination( const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                  const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const
{
    glm::vec3 surfaceToCamera = glm::normalize(eyePosition-surfacePosition);
    glm::vec3 surfaceToLight = this->position() - surfacePosition;

    // Diffuse
    float distance = surfaceToLight.length();
    surfaceToLight = glm::normalize(surfaceToLight);
    float diffuseFactor = max(glm::dot(surfaceNormal, surfaceToLight), 0.0f);

    // Specular
    glm::vec3 reflectDirection = reflect(-surfaceToLight, surfaceNormal);
    float specularFactor = pow(max(glm::dot(surfaceToCamera, reflectDirection), 0.0f), material->shininess());

    // Spotlight (soft edges)
    float cos_theta = glm::dot(surfaceToLight, -this->spotDirection());
    float intensity = clamp( (cos_theta - this->outerCutOff() ) / ( this->innerCutOff() - this->outerCutOff() ), 0.0f, 1.0f );

    // Attenuation
    float attenuation = 1.0f / (this->constant() + this->linear() * distance + this->quadratic() * (distance * distance));

    // Combine results
    glm::vec3 ambient  = attenuation * this->ambient() * material->ambient();
    glm::vec3 diffuse  = intensity * attenuation * diffuseFactor  * this->diffuse() * material->diffuse();
    glm::vec3 specular = intensity * attenuation * specularFactor * this->specular() * material->specular();

    return (ambient + diffuse + specular);
}

glm::vec3 SpotLight::   lightDirectionFrom(const glm::vec3& position) const
{
    return position==m_position ? glm::vec3(0,0,0) : glm::normalize(position-m_position);
}
