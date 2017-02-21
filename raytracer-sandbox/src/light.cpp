#include "./../include/raytracer-sandbox/light.hpp"
#include "./../include/raytracer-sandbox/utils.hpp"

using namespace std;

Light::~Light(){}

DirectionalLight::~DirectionalLight()
{}

DirectionalLight::DirectionalLight()
{
    m_direction = glm::vec3(0,0,0);
    m_ambient = glm::vec3(0,0,0);
    m_diffuse = glm::vec3(0,0,0);
    m_specular = glm::vec3(0,0,0);
}

DirectionalLight::DirectionalLight(const DirectionalLight& directionalLight)
{
    m_direction = directionalLight.m_direction;
    m_ambient = directionalLight.m_ambient;
    m_diffuse = directionalLight.m_diffuse;
    m_specular = directionalLight.m_specular;
}

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
                                               const glm::vec3& surfaceNormal, const PhongMaterialPtr& material)
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

glm::vec3 DirectionalLight::lightDirectionFrom(const glm::vec3 &/*position*/)
{
    return m_direction;
}

PointLight::~PointLight()
{

}

PointLight::PointLight()
{
    m_position = glm::vec3(0,0,0);
    m_ambient = glm::vec3(0,0,0);
    m_diffuse = glm::vec3(0,0,0);
    m_specular = glm::vec3(0,0,0);
    m_constant = 0.0;
    m_linear = 0.0;
    m_quadratic = 0.0;
}

PointLight::PointLight(const PointLight& pointLight)
{
    m_position = pointLight.m_position;
    m_ambient = pointLight.m_ambient;
    m_diffuse = pointLight.m_diffuse;
    m_specular = pointLight.m_specular;
    m_constant = pointLight.m_constant;
    m_linear = pointLight.m_linear;
    m_quadratic = pointLight.m_quadratic;
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
                                  const glm::vec3& surfaceNormal, const PhongMaterialPtr& material)
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

glm::vec3 PointLight::lightDirectionFrom(const glm::vec3& position)
{
    return glm::normalize(position-m_position);
}

SpotLight::~SpotLight()
{

}

SpotLight::SpotLight()
{
    m_position = glm::vec3(0,0,0);
    m_spotDirection = glm::vec3(0,0,0);
    m_ambient = glm::vec3(0,0,0);
    m_diffuse = glm::vec3(0,0,0);
    m_specular = glm::vec3(0,0,0);
    m_constant = 0.0;
    m_linear = 0.0;
    m_quadratic = 0.0;
    m_innerCutOff = 0.0;
    m_outerCutOff = 0.0;
}

SpotLight::SpotLight(const SpotLight& spotLight)
{
    m_position = spotLight.m_position;
    m_spotDirection = glm::normalize(spotLight.m_spotDirection);
    m_ambient = spotLight.m_ambient;
    m_diffuse = spotLight.m_diffuse;
    m_specular = spotLight.m_specular;
    m_constant = spotLight.m_constant;
    m_linear = spotLight.m_linear;
    m_quadratic = spotLight.m_quadratic;
    m_innerCutOff = spotLight.m_innerCutOff;
    m_outerCutOff = spotLight.m_outerCutOff;
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
                                  const glm::vec3& surfaceNormal, const PhongMaterialPtr& material)
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

glm::vec3 SpotLight::lightDirectionFrom(const glm::vec3& position)
{
    return glm::normalize(position-m_position);
}
