#ifndef EXTENT_HPP
#define EXTENT_HPP

#include <vector>
#include <glm/glm.hpp>

class Extent
{
public:
    ~Extent();
    Extent() = default;
    Extent( const Extent& extent ) = default;
    static std::vector<glm::vec3> m_planeSetNormals;
    static void SetPlaneSetNormals(const std::vector<glm::vec3> &planeSetNormals);
private:

};

#endif // EXTENT_HPP
