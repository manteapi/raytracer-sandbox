#include "./../include/raytracer-sandbox/tmesh.hpp"
#include "./../include/raytracer-sandbox/io.hpp"

TMesh::~TMesh(){}

TMesh::TMesh(const std::string& filename, const MaterialPtr& material)
{
    this->material() = material;

    read_obj(filename, m_positions, m_indices, m_normals, m_texCoords);

    glm::vec3 minBB( -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() );
    glm::vec3 maxBB( std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
    for(size_t i=0; i<m_positions.size(); ++i)
    {
        for(size_t j=0; j<3; ++j)
        {
            minBB[j] = m_positions[i][j]<minBB[j] ? m_positions[i][j] : minBB[j];
            maxBB[j] = m_positions[i][j]>maxBB[j] ? m_positions[i][j] : maxBB[j];
        }
    }
    this->bbox() = Box(minBB, maxBB);
}

bool TMesh::Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal)
{
    glm::vec3 barycentricCoords;
    for(size_t i=0; i<m_indices.size()/3; i++)
    {
        if(triangleRayIntersection(m_positions[ m_indices[3*i] ], m_positions[ m_indices[3*i+1] ], m_positions[ m_indices[3*i+2] ], r, hitPosition, hitNormal, barycentricCoords))
        {
            hitNormal = barycentricCoords[0]*m_normals[m_indices[3*i]] + barycentricCoords[1]*m_normals[m_indices[3*i+1]] + barycentricCoords[2]*m_normals[m_indices[3*i+2]];
            hitNormal = glm::normalize(hitNormal);
            return true;
        }
    }
    return false;
}
