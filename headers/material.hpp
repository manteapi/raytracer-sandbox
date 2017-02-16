#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include <QVector3D>

enum MaterialType { GLOSSY, PHONG, FRESNEL };

class Material
{
public:
    virtual ~Material();
    Material() = default;
    virtual MaterialType type() = 0;
};

typedef std::shared_ptr<Material> MaterialPtr;

class GlossyMaterial : public Material
{
public:
    ~GlossyMaterial();
    GlossyMaterial() = default;
    virtual MaterialType type();
};

typedef std::shared_ptr<GlossyMaterial> GlossyMaterialPtr;

class FresnelMaterial : public Material
{
public:
    ~FresnelMaterial();
    FresnelMaterial() = default;
    FresnelMaterial( const FresnelMaterial& material ) = default;
    FresnelMaterial(const float& ior);
    float& ior();
    const float& ior() const;
    //Dielectric fresnel
    virtual MaterialType type();
    static float AirIOR();
    static float WaterIOR();
    static float GlassIOR();
    static float DiamondIOR();

private:
    float m_ior;
};

typedef std::shared_ptr<FresnelMaterial> FresnelMaterialPtr;

/**
 * @brief Material properties of an object for the Phong illumination model.
 *
 * This class represents the material properties of an object for
 * the illumination model of Phong.
 */
class PhongMaterial : public Material
{
public:
    typedef std::shared_ptr<PhongMaterial> MaterialPtr; /*!< Smart pointer to a material */

    /**
     * @brief Destructor
     */
    ~PhongMaterial();

    /**
     * @brief Default constructor
     */
    PhongMaterial();

    /**
     * @brief Default constructor
     */
    PhongMaterial(const PhongMaterial& material);

    /**
     * @brief Specific constructor
     *
     * Construct a material
     *
     * @param ambient The ambient vector of the material.
     * @param diffuse The diffuse vector of the material.
     * @param specular The specular vector of the material.
     * @param shininess The shininess coefficient of the material.
     */
    PhongMaterial(const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular, const float& shininess);

    /**
     * @brief Access to the ambient vector of the material.
     *
     * @return A const reference to m_ambient.
     */
    const QVector3D& ambient() const;

    /**
     * @brief Set the ambient vector of the material.
     *
     * Set the value of m_ambient.
     * @param ambient The new ambient vector of the material.
     */
    void setAmbient(const QVector3D &ambient);

    /**
     * @brief Access to the diffuse vector of the material.
     *
     * @return A const reference to m_diffuse.
     */
    const QVector3D& diffuse() const;

    /**
     * @brief Set the diffuse vector of the material.
     *
     * Set the value of m_diffuse.
     * @param diffuse The new diffuse vector of the material.
     */
    void setDiffuse(const QVector3D &diffuse);

    /**
     * @brief Access to the specular vector of the material.
     *
     * @return A const reference to m_specular.
     */
    const QVector3D& specular() const;

    /**
     * @brief Set the specular vector of the material.
     *
     * Set the value of m_specular.
     * @param specular The new specular vector of the material.
     */
    void setSpecular(const QVector3D &specular);

    /**
     * @brief Access to the shininess coefficient of the material.
     *
     * @return A const reference to m_shininess.
     */
    const float& shininess() const;

    /**
     * @brief Set the shininess coefficient of the material.
     *
     * Set the value of m_shininess.
     * @param shininess The new shininess vector of the material.
     */
    void setShininess(float shininess);

    /**
     * @brief Construct a pearl material from real data according to http://devernay.free.fr/cours/opengl/materials.html
     * @return A pearl material.
     */
    static MaterialPtr Pearl();

    /**
     * @brief Construct a emerald material from real data according to http://devernay.free.fr/cours/opengl/materials.html
     * @return A emerald material.
     */
    static MaterialPtr Emerald();

    /**
     * @brief Construct a bronze material from real data according to http://devernay.free.fr/cours/opengl/materials.html
     * @return A bronze material.
     */
    static MaterialPtr Bronze();

    virtual MaterialType type();

private:
    QVector3D m_ambient; /*!< The ambient material vector defines what color this object reflects under ambient lighting. */
    QVector3D m_diffuse; /*!< The diffuse material vector defines the color of the object under diffuse lighting. */
    QVector3D m_specular; /*!< The specular material vector sets the color impact a specular light has on the object. */
    float m_shininess; /*!< The shininess impacts the scattering/radius of the specular highlight. */
};

typedef std::shared_ptr<PhongMaterial> PhongMaterialPtr; /*!< Smart pointer to a material */

#endif //MATERIAL_HPP
