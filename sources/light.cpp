#include "./../headers/light.hpp"
#include "utils.hpp"

using namespace std;

Light::~Light(){}

DirectionalLight::~DirectionalLight()
{}

DirectionalLight::DirectionalLight()
{
    m_direction = QVector3D(0,0,0);
    m_ambient = QVector3D(0,0,0);
    m_diffuse = QVector3D(0,0,0);
    m_specular = QVector3D(0,0,0);
}

DirectionalLight::DirectionalLight(const DirectionalLight& directionalLight)
{
    m_direction = directionalLight.m_direction;
    m_ambient = directionalLight.m_ambient;
    m_diffuse = directionalLight.m_diffuse;
    m_specular = directionalLight.m_specular;
}

DirectionalLight::DirectionalLight(const QVector3D& direction, const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular)
{
    m_direction = direction.normalized();
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
}

const QVector3D &DirectionalLight::direction() const
{
    return m_direction;
}

void DirectionalLight::setDirection(const QVector3D &direction)
{
    m_direction = direction.normalized();
}

const QVector3D& DirectionalLight::ambient() const
{
    return m_ambient;
}

void DirectionalLight::setAmbient(const QVector3D &ambient)
{
    m_ambient = ambient;
}

const QVector3D &DirectionalLight::diffuse() const
{
    return m_diffuse;
}

void DirectionalLight::setDiffuse(const QVector3D &diffuse)
{
    m_diffuse = diffuse;
}

const QVector3D& DirectionalLight::specular() const
{
    return m_specular;
}

void DirectionalLight::setSpecular(const QVector3D &specular)
{
    m_specular = specular;
}

QVector3D DirectionalLight::phongIllumination( const QVector3D& eyePosition, const QVector3D& surfacePosition,
                                               const QVector3D& surfaceNormal, const PhongMaterialPtr& material)
{
    QVector3D surfaceToCamera = (eyePosition-surfacePosition).normalized();
    QVector3D surfaceToLight = -this->direction();

    // Diffuse shading
    float diffuseFactor = max(QVector3D::dotProduct(surfaceNormal, surfaceToLight), 0.0f);

    // Specular shading
    QVector3D reflectDirection = reflect(-surfaceToLight, surfaceNormal);
    float specularFactor = pow(max(QVector3D::dotProduct(surfaceToCamera, reflectDirection), 0.0f), material->shininess());

    // Combine results
    QVector3D ambient  =                  this->ambient() * material->ambient();
    QVector3D diffuse  = diffuseFactor  * this->diffuse() * material->diffuse();
    QVector3D specular = specularFactor * this->specular() * material->specular();

    return (ambient + diffuse + specular);
}

QVector3D DirectionalLight::lightDirectionFrom(const QVector3D &/*position*/)
{
    return m_direction;
}

PointLight::~PointLight()
{

}

PointLight::PointLight()
{
    m_position = QVector3D(0,0,0);
    m_ambient = QVector3D(0,0,0);
    m_diffuse = QVector3D(0,0,0);
    m_specular = QVector3D(0,0,0);
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

PointLight::PointLight(const QVector3D &position, const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular,
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

const QVector3D &PointLight::position() const
{
    return m_position;
}

void PointLight::setPosition(const QVector3D &position)
{
    m_position = position;
}

const QVector3D& PointLight::ambient() const
{
    return m_ambient;
}

void PointLight::setAmbient(const QVector3D &ambient)
{
    m_ambient = ambient;
}

const QVector3D &PointLight::diffuse() const
{
    return m_diffuse;
}

void PointLight::setDiffuse(const QVector3D &diffuse)
{
    m_diffuse = diffuse;
}

const QVector3D& PointLight::specular() const
{
    return m_specular;
}

void PointLight::setSpecular(const QVector3D &specular)
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

QVector3D PointLight::phongIllumination( const QVector3D& eyePosition, const QVector3D& surfacePosition,
                                  const QVector3D& surfaceNormal, const PhongMaterialPtr& material)
{
    QVector3D surfaceToCamera = (eyePosition-surfacePosition).normalized();
    QVector3D surfaceToLight = this->position() - surfacePosition;

    // Diffuse shading
    float distance = surfaceToLight.length();
    surfaceToLight.normalize();
    float diffuseFactor = max(QVector3D::dotProduct(surfaceNormal, surfaceToLight), 0.0f);

    // Specular shading
    QVector3D reflectDirection = reflect(-surfaceToLight, surfaceNormal);
    float specularFactor = pow(max(QVector3D::dotProduct(surfaceToCamera, reflectDirection), 0.0f), material->shininess());

    // Attenuation
    float attenuation = 1.0f / (this->constant() + this->linear() * distance + this->quadratic() * (distance * distance));

    // Combine results
    QVector3D ambient  = attenuation * this->ambient() * material->ambient();
    QVector3D diffuse  = attenuation * diffuseFactor * this->diffuse() * material->diffuse();
    QVector3D specular = attenuation * specularFactor * this->specular() * material->specular();

    QVector3D color = ambient + diffuse + specular;
    return color;
}

QVector3D PointLight::lightDirectionFrom(const QVector3D& position)
{
    return (position-m_position).normalized();
}

SpotLight::~SpotLight()
{

}

SpotLight::SpotLight()
{
    m_position = QVector3D(0,0,0);
    m_spotDirection = QVector3D(0,0,0);
    m_ambient = QVector3D(0,0,0);
    m_diffuse = QVector3D(0,0,0);
    m_specular = QVector3D(0,0,0);
    m_constant = 0.0;
    m_linear = 0.0;
    m_quadratic = 0.0;
    m_innerCutOff = 0.0;
    m_outerCutOff = 0.0;
}

SpotLight::SpotLight(const SpotLight& spotLight)
{
    m_position = spotLight.m_position;
    m_spotDirection = spotLight.m_spotDirection.normalized();
    m_ambient = spotLight.m_ambient;
    m_diffuse = spotLight.m_diffuse;
    m_specular = spotLight.m_specular;
    m_constant = spotLight.m_constant;
    m_linear = spotLight.m_linear;
    m_quadratic = spotLight.m_quadratic;
    m_innerCutOff = spotLight.m_innerCutOff;
    m_outerCutOff = spotLight.m_outerCutOff;
}

SpotLight::SpotLight(const QVector3D& position, const QVector3D& spotDirection,
          const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular,
          const float& constant, const float& linear, const float& quadratic,
          const float& innerCutOff, const float& outerCutOff)
{
    m_position = position;
    m_spotDirection = spotDirection.normalized();
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
    m_innerCutOff = innerCutOff;
    m_outerCutOff = outerCutOff;
}

const QVector3D &SpotLight::position() const
{
    return m_position;
}

void SpotLight::setPosition(const QVector3D &position)
{
    m_position = position;
}

const QVector3D &SpotLight::spotDirection() const
{
    return m_spotDirection;
}

void SpotLight::setSpotDirection(const QVector3D &spotDirection)
{
    m_spotDirection = spotDirection.normalized();
}

const QVector3D &SpotLight::ambient() const
{
    return m_ambient;
}

void SpotLight::setAmbient(const QVector3D &ambient)
{
    m_ambient = ambient;
}

const QVector3D& SpotLight::diffuse() const
{
    return m_diffuse;
}

void SpotLight::setDiffuse(const QVector3D &diffuse)
{
    m_diffuse = diffuse;
}

const QVector3D &SpotLight::specular() const
{
    return m_specular;
}

void SpotLight::setSpecular(const QVector3D &specular)
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

QVector3D SpotLight::phongIllumination( const QVector3D& eyePosition, const QVector3D& surfacePosition,
                                  const QVector3D& surfaceNormal, const PhongMaterialPtr& material)
{
    QVector3D surfaceToCamera = (eyePosition-surfacePosition).normalized();
    QVector3D surfaceToLight = this->position() - surfacePosition;

    // Diffuse
    float distance = surfaceToLight.length();
    surfaceToLight.normalize();
    float diffuseFactor = max(QVector3D::dotProduct(surfaceNormal, surfaceToLight), 0.0f);

    // Specular
    QVector3D reflectDirection = reflect(-surfaceToLight, surfaceNormal);
    float specularFactor = pow(max(QVector3D::dotProduct(surfaceToCamera, reflectDirection), 0.0f), material->shininess());

    // Spotlight (soft edges)
    float cos_theta = QVector3D::dotProduct(surfaceToLight, -this->spotDirection());
    float intensity = clamp( (cos_theta - this->outerCutOff() ) / ( this->innerCutOff() - this->outerCutOff() ), 0.0f, 1.0f );

    // Attenuation
    float attenuation = 1.0f / (this->constant() + this->linear() * distance + this->quadratic() * (distance * distance));

    // Combine results
    QVector3D ambient  = attenuation * this->ambient() * material->ambient();
    QVector3D diffuse  = intensity * attenuation * diffuseFactor  * this->diffuse() * material->diffuse();
    QVector3D specular = intensity * attenuation * specularFactor * this->specular() * material->specular();

    return (ambient + diffuse + specular);
}

QVector3D SpotLight::lightDirectionFrom(const QVector3D& position)
{
    return (position-m_position).normalized();
}
