#include "./../include/raytracer-sandbox/pointLight.hpp"

using namespace std;

PointLight::~PointLight()
{

}

PointLight::PointLight(const glm::vec3 &position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                       const float& constant, const float& linear, const float& quadratic)
{
    m_position = position;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
}

const glm::vec3 &PointLight::position() const
{
    return m_position;
}

void PointLight::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

const glm::vec3& PointLight::ambient() const
{
    return m_ambient;
}

void PointLight::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3 &PointLight::diffuse() const
{
    return m_diffuse;
}

void PointLight::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& PointLight::specular() const
{
    return m_specular;
}

void PointLight::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

const float &PointLight::constant() const
{
    return m_constant;
}

void PointLight::setConstant(float constant)
{
    m_constant = constant;
}

const float& PointLight::linear() const
{
    return m_linear;
}

void PointLight::setLinear(float linear)
{
    m_linear = linear;
}

const float &PointLight::quadratic() const
{
    return m_quadratic;
}

void PointLight::setQuadratic(float quadratic)
{
    m_quadratic = quadratic;
}

glm::vec3 PointLight::phongIllumination( const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                         const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const
{
    glm::vec3 surfaceToCamera = glm::normalize(eyePosition-surfacePosition);
    glm::vec3 surfaceToLight = this->position() - surfacePosition;

    // Diffuse shading
    float distance = surfaceToLight.length();
    surfaceToLight = glm::normalize(surfaceToLight);
    float diffuseFactor = max(glm::dot(surfaceNormal, surfaceToLight), 0.0f);

    // Specular shading
    glm::vec3 reflectDirection = reflect(-surfaceToLight, surfaceNormal);
    float specularFactor = pow(max(glm::dot(surfaceToCamera, reflectDirection), 0.0f), material->shininess());

    // Attenuation
    float attenuation = 1.0f / (this->constant() + this->linear() * distance + this->quadratic() * (distance * distance));

    // Combine results
    glm::vec3 ambient  = attenuation * this->ambient() * material->ambient();
    glm::vec3 diffuse  = attenuation * diffuseFactor * this->diffuse() * material->diffuse();
    glm::vec3 specular = attenuation * specularFactor * this->specular() * material->specular();

    glm::vec3 color = ambient + diffuse + specular;
    return color;
}

glm::vec3 PointLight::lightDirectionFrom(const glm::vec3& position) const
{
    return glm::normalize(position-m_position);
}
