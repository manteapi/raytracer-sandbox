#include "./../headers/material.hpp"
#include "utils.hpp"

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

PhongMaterial::PhongMaterial()
{
    m_ambient = QVector3D(0.0,0.0,0.0);
    m_diffuse = QVector3D(0.0,0.0,0.0);
    m_specular = QVector3D(0.0,0.0,0.0);
    m_shininess = 0.0;
}

PhongMaterial::PhongMaterial(const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular, const float &shininess)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = shininess;
}

PhongMaterial::PhongMaterial(const PhongMaterial& material)
{
    m_ambient = material.m_ambient;
    m_diffuse = material.m_diffuse;
    m_specular = material.m_specular;
    m_shininess = material.m_shininess;
}

const QVector3D& PhongMaterial::ambient() const
{
    return m_ambient;
}

void PhongMaterial::setAmbient(const QVector3D &ambient)
{
    m_ambient = ambient;
}

const QVector3D& PhongMaterial::diffuse() const
{
    return m_diffuse;
}

void PhongMaterial::setDiffuse(const QVector3D &diffuse)
{
    m_diffuse = diffuse;
}

const QVector3D& PhongMaterial::specular() const
{
    return m_specular;
}

void PhongMaterial::setSpecular(const QVector3D &specular)
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
    QVector3D ambient(0.25, 0.20725, 0.20725);
    QVector3D diffuse(1.0, 0.829, 0.829);
    QVector3D specular(0.296648, 0.296648, 0.296648);
    float shininess = openGLFactor*0.088;
    return std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);
}

PhongMaterialPtr PhongMaterial::Emerald()
{
    float openGLFactor=128.0;
    QVector3D ambient(0.0215, 0.1745, 0.0215);
    QVector3D diffuse(0.07568, 0.61424, 0.07568);
    QVector3D specular(0.633, 0.727811, 0.633);
    float shininess = openGLFactor*0.6;
    return std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);
}

PhongMaterialPtr PhongMaterial::Bronze()
{
    float openGLFactor=128.0;
    QVector3D ambient(0.2125, 0.1275, 0.054);
    QVector3D diffuse(0.714, 0.4284, 0.18144);
    QVector3D specular(0.393548, 0.271906, 0.166721);
    float shininess = openGLFactor*0.2;
    return std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);
}

MaterialType PhongMaterial::type()
{
    return PHONG;
}
