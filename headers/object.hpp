#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <array>
#include <QVector3D>
#include "material.hpp"
#include "ray.hpp"
#include "utils.hpp"

class Box
{
public:
    ~Box() = default;
    Box() = default;
    Box(const QVector3D& minBounds, const QVector3D& maxBounds);
    Box(const Box& box) = default;
    std::array< QVector3D, 2 >& bounds();
    const std::array< QVector3D, 2 >& bounds() const;
    QVector3D& maxExtent();
    const QVector3D& maxExtent() const;
    QVector3D& minExtent();
    const QVector3D& minExtent() const;
private:
    std::array< QVector3D, 2 > m_bounds;
};

bool Intersect(const Ray &r, const Box& box, std::array<float, 2> &t);

class Object
{
public:
    Object() = default;
    Object(const Object& object) = default;
    virtual ~Object();
    virtual bool Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal) = 0;
    MaterialPtr& material();
    const MaterialPtr& material() const;
    Box& bbox();
    const Box& bbox() const;
protected:
    MaterialPtr m_material;
    Box m_bbox;
};

typedef std::shared_ptr<Object> ObjectPtr;

/** @brief Geometry representation of a triangular mesh.
 *
 * Represent a triangular mesh.
 */
class TMesh : public Object
{
public:
    /**
     * @brief Destructor
     */
    ~TMesh();

    /**
     * @brief Default constructor
     */
    TMesh() = delete;

    /** @brief Build a triangular mesh from an obj file and a specific material.
     *
     * Build a triangular mesh from an obj file and a specific material.
     * @param filename The path to the obj file.
     * @param material A reference to the material's pointer.
     */
    TMesh(const std::string& filename, const MaterialPtr &material);

    /**
     * @brief Clone constructor
     */
    TMesh(const TMesh& mesh) = default;

    /** @brief Compute the intersection between the object and a ray.
     *
     * Compute the intersection between the object and a ray. It return true if an intersection occured or false otherwise.
     * The position of the intersection and the normal of the surface at the position of intersection are written in the referenced parameters.
     * @param r The ray tested for intersection.
     * @param hitPosition The position of the intersection.
     * @param hitNormal The normal of the surface at the position of the intersection.
     * @return True if intersection occured and False otherwise.
     */
    virtual bool Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal);

private:
    std::vector<unsigned int> m_indices; /*!< The indices of the triangles of the mesh. For instance, the indices of a triangle i are m_indices[3*i+0], m_indices[3*i+1] and m_indices[3*i+2]. */
    std::vector<QVector2D> m_texCoords; /*!< The texture coordinates of the vertices of the mesh. */
    std::vector<QVector3D> m_positions; /*!< The positions of the vertices of the mesh. */
    std::vector<QVector3D> m_normals; /*!< The normals of the vertices of the mesh. */
};

/** @brief Geometry representation of an infinite plane.
 *
 * Represent an infinite plane.
 */
class Plane : public Object
{
public:
    Plane() = default;
    Plane(const Plane& plane) = default;

  /** @brief Build a plane with a normal and a point.
   *
   * Build a plane normal to the specified normal, passing by a particular
   * point.
   * @param normal The normal of the plane
   * @param point A point belonging to the plane.
   */
  Plane( const QVector3D& normal,
         const QVector3D& point, const MaterialPtr& material);

  /** @brief Build the plane passing through three points.
   *
   * Build a plane containing three points.
   * @param a The first point belonging to the plane.
   * @param b The second point belonging to the plane.
   * @param c The third point belonging to the plane.
   */
  Plane(const QVector3D& a, const QVector3D& b, const QVector3D& c, const MaterialPtr& material);

  /** @brief Manually set the distance from this plane to the origin.
   *
   * Set the distance min_(p in this) || p - vec3(0,0,0) ||.
   * @param d New distance to the origin.
   */
  void setDistanceToOrigin(const float& d);
  /** @brief Access to the distance to the origin.
   *
   * Get the distance between this plane and the origin.
   * @return The distance to the origin.
   */
  const float& distanceToOrigin() const;

  /** @brief Set the plane's normal.
   *
   * Set the plane's normal.
   * @param n The new plane's normal.
   */
  void setNormal(const QVector3D& n);
  /** @brief Access the plane's normal.
   *
   * Get the plane's normal.
   * @return The plane's normal.
   */
  const QVector3D& normal() const;

  /** @brief Get the projection of a point on this plane.
   *
   * Get the orthogonal projection of a point on this plane.
   * @param p The point to project.
   * @return The orthogonal projection of p.
   */
  QVector3D projectOnPlane(const QVector3D& p);

  virtual bool Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal);

private:
    QVector3D m_n; /*!< Plane normal. Points x on the plane satisfy dot(m_n,x)=m_d */
    float m_d; /*!< m_d = dot(m_n,p) for a given point p on the plane */
};

typedef std::shared_ptr<Plane> PlanePtr;

std::ostream& operator << ( std::ostream& out, const Plane& o);

typedef std::shared_ptr<Plane> PlanePtr;

class Sphere : public Object
{
public:
    ~Sphere();
    Sphere(const QVector3D& position, const float& radius, const MaterialPtr& material);
    Sphere() = default;
    Sphere(const Sphere& s) = default;
    QVector3D& position();
    const QVector3D& position() const;
    float& radius();
    const float& radius() const;
    virtual bool Intersect(const Ray& r, QVector3D& hitPosition, QVector3D& hitNormal);
private:
    float m_radius;
    QVector3D m_position;
};

std::ostream& operator << ( std::ostream& out, const Sphere& o);

typedef std::shared_ptr<Sphere> SpherePtr;

#endif //OBJECT_HPP
