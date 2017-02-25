#include "./../include/raytracer-sandbox/directionalLight.hpp"

using namespace std;

DirectionalLight::~DirectionalLight()
{}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
{
    m_direction = glm::normalize(direction);
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
}

const glm::vec3 &DirectionalLight::direction() const
{
    return m_direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction)
{
    m_direction = glm::normalize(direction);
}

const glm::vec3& DirectionalLight::ambient() const
{
    return m_ambient;
}

void DirectionalLight::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3 &DirectionalLight::diffuse() const
{
    return m_diffuse;
}

void DirectionalLight::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& DirectionalLight::specular() const
{
    return m_specular;
}

void DirectionalLight::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

glm::vec3 DirectionalLight::phongIllumination( const glm::vec3& eyePosition, const glm::vec3& surfacePosition,
                                               const glm::vec3& surfaceNormal, const PhongMaterialPtr& material) const
{
    glm::vec3 surfaceToCamera = glm::normalize(eyePosition-surfacePosition);
    glm::vec3 surfaceToLight = -this->direction();

    // Diffuse shading
    float diffuseFactor = max(glm::dot(surfaceNormal, surfaceToLight), 0.0f);

    // Specular shading
    glm::vec3 reflectDirection = reflect(-surfaceToLight, surfaceNormal);
    float specularFactor = pow(max(glm::dot(surfaceToCamera, reflectDirection), 0.0f), material->shininess());

    // Combine results
    glm::vec3 ambient  =                  this->ambient() * material->ambient();
    glm::vec3 diffuse  = diffuseFactor  * this->diffuse() * material->diffuse();
    glm::vec3 specular = specularFactor * this->specular() * material->specular();

    return (ambient + diffuse + specular);
}

glm::vec3 DirectionalLight::lightDirectionFrom(const glm::vec3 &/*position*/) const
{
    return m_direction;
}
