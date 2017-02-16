#include "./../headers/camera.hpp"
#include <iostream>
#include <QVector2D>

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

    m_view.setToIdentity();

    m_projection.setToIdentity();
    m_projection.perspective(m_fov, m_ratio, m_znear, m_zfar);
}

Camera::Camera()
{
    m_view.setToIdentity();
}

QMatrix4x4& Camera::view()
{
    return m_view;
}

const QMatrix4x4& Camera::view() const
{
    return m_view;
}

float& Camera::fov()
{
    return m_fov;
}

const float& Camera::fov() const
{
    return m_fov;
}

float& Camera::ratio()
{
    return m_ratio;
}

const float& Camera::ratio() const
{
    return m_ratio;
}

float& Camera::znear()
{
    return m_znear;
}

const float& Camera::znear() const
{
    return m_znear;
}

float& Camera::zfar()
{
    return m_zfar;
}

const float& Camera::zfar() const
{
    return m_zfar;
}

int& Camera::width()
{
    return m_width;
}

const int& Camera::width() const
{
    return m_width;
}

int& Camera::height()
{
    return m_height;
}

const int& Camera::height() const
{
    return m_height;
}

QVector3D Camera::getPosition() const
{
    QMatrix4x4 invView = m_view.inverted();
    return invView.column(3).toVector3D();
}

QVector3D Camera::pixelToWorld( const float& xCoord, const float& yCoord) const
{
    QMatrix4x4 invProjection = m_projection.inverted();
    QMatrix4x4 invView = m_view.inverted();

    QVector2D v(0,0); //Viewport coordinate
    QVector3D pixelCoord(xCoord, yCoord, m_znear);
    QVector3D ndcCoord;
    ndcCoord[0] = (2.0 /(float)m_width)*(pixelCoord[0]-v[0])-1;
    ndcCoord[1] = 1.0-(2.0 /(float)m_height)*(pixelCoord[1]-v[1]);
    ndcCoord[2] = (2.0 / (m_zfar-m_znear))*(pixelCoord[2]-((m_zfar+m_znear)/2.0));
    QVector4D clipCoord = QVector4D(ndcCoord, m_znear);
    QVector4D eyeCoord = invProjection*clipCoord;
    QVector4D worldCoord = invView*eyeCoord;
    return worldCoord.toVector3D();
}

Ray Camera::computeRayThroughPixel(const float &x, const float &y)
{
    QVector3D origin = getPosition();
    QVector3D pixelWorld = pixelToWorld(x,y);
    QVector3D direction = pixelWorld-origin;
    return Ray( origin, direction.normalized() );
}

ostream& operator << ( ostream& out, const Camera& camera )
{
    out << "FOV : " << camera.fov() << endl;
    out << "Ratio : " << camera.ratio() << endl;
    out << "width : " << camera.width() << endl;
    out << "height : " << camera.height() << endl;
    out << "znear: " << camera.znear() << endl;
    out << "zfar: " << camera.zfar() << endl;
    QVector3D cameraPosition = camera.getPosition();
    out << "Camera position : " << cameraPosition[0] << ", " << cameraPosition[1] << ", " << cameraPosition[2] << endl;
    QVector3D screenPosition = camera.pixelToWorld((int)(camera.width()/2.0f), (int)(camera.height()/2.0f));
    out << "Screen center position : " << screenPosition[0] << ", " << screenPosition[1] << ", " << screenPosition[2] << endl;
    return out;
}
