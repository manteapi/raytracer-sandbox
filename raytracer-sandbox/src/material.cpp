#include "./../include/raytracer-sandbox/material.hpp"
#include "./../include/raytracer-sandbox/utils.hpp"

Material::~Material(){}

GlossyMaterial::~GlossyMaterial(){}

MaterialType GlossyMaterial::type()
{
    return GLOSSY;
}

FresnelMaterial::~FresnelMaterial(){}

FresnelMaterial::FresnelMaterial(const float& ior)
{
    m_ior = ior;
}

float& FresnelMaterial::ior()
{
    return m_ior;
}

const float& FresnelMaterial::ior() const
{
    return m_ior;
}

MaterialType FresnelMaterial::type()
{
    return FRESNEL;
}

float FresnelMaterial::AirIOR()
{
    return 1.0;
}

float FresnelMaterial::WaterIOR()
{
    return 1.3;
}

float FresnelMaterial::GlassIOR()
{
    return 1.5;
}

float FresnelMaterial::DiamondIOR()
{
    return 1.8;
}

PhongMaterial::~PhongMaterial()
{}

PhongMaterial::PhongMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float &shininess)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = shininess;
}

const glm::vec3& PhongMaterial::ambient() const
{
    return m_ambient;
}

void PhongMaterial::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3& PhongMaterial::diffuse() const
{
    return m_diffuse;
}

void PhongMaterial::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& PhongMaterial::specular() const
{
    return m_specular;
}

void PhongMaterial::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

void PhongMaterial::setShininess(float shininess)
{
    m_shininess = shininess;
}

const float &PhongMaterial::shininess() const
{
    return m_shininess;
}

PhongMaterialPtr PhongMaterial::Pearl()
{
    float openGLFactor=128.0;
    glm::vec3 ambient(0.25, 0.20725, 0.20725);
    glm::vec3 diffuse(1.0, 0.829, 0.829);
    glm::vec3 specular(0.296648, 0.296648, 0.296648);
    float shininess = openGLFactor*0.088;
    return std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);
}

PhongMaterialPtr PhongMaterial::Emerald()
{
    float openGLFactor=128.0;
    glm::vec3 ambient(0.0215, 0.1745, 0.0215);
    glm::vec3 diffuse(0.07568, 0.61424, 0.07568);
    glm::vec3 specular(0.633, 0.727811, 0.633);
    float shininess = openGLFactor*0.6;
    return std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);
}

PhongMaterialPtr PhongMaterial::Bronze()
{
    float openGLFactor=128.0;
    glm::vec3 ambient(0.2125, 0.1275, 0.054);
    glm::vec3 diffuse(0.714, 0.4284, 0.18144);
    glm::vec3 specular(0.393548, 0.271906, 0.166721);
    float shininess = openGLFactor*0.2;
    return std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);
}

MaterialType PhongMaterial::type()
{
    return PHONG;
}
