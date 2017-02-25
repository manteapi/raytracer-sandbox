#ifndef TMESH_HPP
#define TMESH_HPP

#include "object.hpp"
#include <vector>
#include <glm/glm.hpp>

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
    virtual bool Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal);

private:
    std::vector<unsigned int> m_indices; /*!< The indices of the triangles of the mesh. For instance, the indices of a triangle i are m_indices[3*i+0], m_indices[3*i+1] and m_indices[3*i+2]. */
    std::vector<glm::vec2> m_texCoords; /*!< The texture coordinates of the vertices of the mesh. */
    std::vector<glm::vec3> m_positions; /*!< The positions of the vertices of the mesh. */
    std::vector<glm::vec3> m_normals; /*!< The normals of the vertices of the mesh. */
};

#endif // TMESH_HPP
