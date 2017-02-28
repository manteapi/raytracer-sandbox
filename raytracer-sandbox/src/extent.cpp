#include "./../include/raytracer-sandbox/extent.hpp"

Extent::~Extent(){}

void Extent::SetPlaneSetNormals(const std::vector<glm::vec3>& planeSetNormals)
{
    m_planeSetNormals = planeSetNormals;
}
