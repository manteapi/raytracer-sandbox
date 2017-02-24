#include "./../include/raytracer-sandbox/camera.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using namespace  std;

Camera::~Camera()
{

}

Camera::Camera(float fov, int width, int height, float near, float far)
{
    m_fov = fov;
    m_width = width;
    m_height = height;
    m_ratio = (float)width/(float)height;
    m_znear = near;
    m_zfar = far;

    m_view=glm::mat4(1.0f);

    m_projection=glm::mat4(1.0f);
    m_projection = glm::perspective(m_fov, m_ratio, m_znear, m_zfar);
}

glm::mat4& Camera::view()
{
    return m_view;
}

const glm::mat4& Camera::view() const
{
    return m_view;
}

const glm::mat4& Camera::projection() const
{
    return m_projection;
}

const float& Camera::fov() const
{
    return m_fov;
}

const float& Camera::ratio() const
{
    return m_ratio;
}

const float& Camera::znear() const
{
    return m_znear;
}

const float& Camera::zfar() const
{
    return m_zfar;
}

const int& Camera::width() const
{
    return m_width;
}

const int& Camera::height() const
{
    return m_height;
}

glm::vec3 Camera::computePosition() const
{
    glm::mat4 invView = glm::inverse(m_view);
    return glm::vec3(invView[3][0], invView[3][1], invView[3][2]);
}

glm::vec3 Camera::pixelToWorld( const float& xCoord, const float& yCoord) const
{
    glm::mat4 invProjection = glm::inverse(m_projection);
    glm::mat4 invView = glm::inverse(m_view);

    glm::vec2 v(0,0); //Viewport coordinate
    glm::vec3 pixelCoord(xCoord, yCoord, m_znear);
    glm::vec3 ndcCoord;
    ndcCoord[0] = (2.0 /(float)m_width)*(pixelCoord[0]-v[0])-1;
    //ndcCoord[1] = (2.0 /(float)m_height)*(pixelCoord[1]-v[1])-1;
    ndcCoord[1] = 1.0-(2.0 /(float)m_height)*(pixelCoord[1]-v[1]);
    ndcCoord[2] = (2.0 / (m_zfar-m_znear))*(pixelCoord[2]-((m_zfar+m_znear)/2.0));
    glm::vec4 clipCoord = glm::vec4(ndcCoord, m_znear);
    glm::vec4 eyeCoord = invProjection*clipCoord;
    glm::vec4 worldCoord = invView*eyeCoord;
    return glm::vec3(worldCoord);
}

Ray Camera::computeRayThroughPixel(const float &x, const float &y)
{
    glm::vec3 origin = computePosition();
    glm::vec3 pixelWorld = pixelToWorld(x,y);
    glm::vec3 direction = pixelWorld-origin;
    return Ray( origin, glm::normalize(direction) );
}

ostream& operator << ( ostream& out, const Camera& camera )
{
    out << "FOV : " << camera.fov() << endl;
    out << "Ratio : " << camera.ratio() << endl;
    out << "width : " << camera.width() << endl;
    out << "height : " << camera.height() << endl;
    out << "znear: " << camera.znear() << endl;
    out << "zfar: " << camera.zfar() << endl;
    glm::vec3 cameraPosition = camera.computePosition();
    out << "Camera position : " << cameraPosition[0] << ", " << cameraPosition[1] << ", " << cameraPosition[2] << endl;
    glm::vec3 screenPosition = camera.pixelToWorld((int)(camera.width()/2.0f), (int)(camera.height()/2.0f));
    out << "Screen center position : " << screenPosition[0] << ", " << screenPosition[1] << ", " << screenPosition[2] << endl;
    return out;
}
