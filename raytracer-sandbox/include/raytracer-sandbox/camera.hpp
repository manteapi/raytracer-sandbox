#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Camera
{
public:

    ~Camera();
    Camera();
    Camera(float fov, int width, int height, float near, float far);
    glm::mat4& view();
    const glm::mat4& view() const;

    float& fov();
    const float& fov() const;

    float& ratio();
    const float& ratio() const;

    float& znear();
    const float& znear() const;

    float& zfar();
    const float& zfar() const;

    int& width();
    const int& width() const;

    int& height();
    const int& height() const;

    glm::vec3 getPosition() const;
    glm::vec3 pixelToWorld(const float &xCoord, const float &yCoord) const;
    Ray computeRayThroughPixel(const float& x, const float& y);

private:
    glm::mat4 m_view, m_projection;
    float m_fov;
    float m_ratio;
    float m_znear;
    float m_zfar;
    int m_width;
    int m_height;
};

std::ostream& operator << ( std::ostream& out, const Camera& camera );

#endif //CAMERA_HPP
