#ifndef CAMERA_HPP
#define CAMERA_HPP

/** @file
 * @brief Define a camera object.
 *
 * Define a camera object to ease the manipulation of the view/projection matrix by user.
 */

#include "ray.hpp"
#include <glm/glm.hpp>
#include <iostream>

/** @brief Manage the Camera.
 *
 * We consider a camera to be defined by two 4x4 matrices:
 *  - the view matrix
 *  - the projection matrix.
 *
 * The view matrix defines the position and the orientation of the camera in the
 * world coordinates. It is represented as follow:
 * | Column 0 | Column 1 | Column 2 | Column 3 |
 * |----------|----------|----------|----------|
 * |    X0    |    X1    |    X2    |    P0    |
 * |    Y0    |    Y1    |    Y2    |    P1    |
 * |    Z0    |    Z1    |    Z2    |    P2    |
 * |    0     |    0     |    0     |    1     |
 * X=(X0,X1,X2), Y=(Y0,Y1,Y2) and Z=(Z0,Z1,Z2) represent the right, up, and
 * backward direction of the camera in the world coordinates. The point
 * P=(P0,P1,P2) is the position of the world origin in the camera frame
 * (X,Y,Z). Thus, the point (-dot(P,X), -dot(P,Y), -dot(P,Z)) is the position
 * of the camera in the world coordinates.
 *
 * The projection matrix defines the way the scene viewed by the camera is
 * transformed (projected) to a 2D image that will be displayed on screen. To
 * compute this matrix, we need ratio of the 2D image, the field of view and
 * the far and near cutting plane distances.
 */
class Camera
{
public:
    /**
     * @brief Destructor
     */
    ~Camera();

    /**
     * @brief Default constructor
     */
    Camera() = default;

    /**
     * @brief Clone constructor
     */
    Camera(const Camera& camera) = default;

    /**
     * @brief Specific constructor
     *
     * @param fov The camera field of view, also called vertical angle, in radians.
     * @param width The width of the display window, in pixel.
     * @param height The height of the display window, in pixel.
     * @param near The distance to the near clipping plane.
     * @param far The distance to the far clipping plane.
     */
    Camera(float fov, int width, int height, float near, float far);

    /**
     * @brief Access to the view matrix of the camera.
     *
     * @return A const reference to m_view.
     */
    const glm::mat4& view() const;

    /**
     * @brief Access to the view matrix of the camera.
     *
     * @return A reference to m_view.
     */
    glm::mat4& view();

    /**
     * @brief Access to the projection matrix of the camera.
     *
     * @return A const reference to m_projection.
     */
    const glm::mat4& projection() const;

    /**
     * @brief Access to the field of view of the camera.
     *
     * @return A const reference to m_fov.
     */
    const float& fov() const;

    /**
     * @brief Access to the ratio of the displayed window handled by the camera.
     *
     * @return A const reference to m_ratio.
     */
    const float& ratio() const;

    /**
     * @brief Access to the distance to the near clipping plane of the camera.
     *
     * @return A const reference to m_znear.
     */
    const float& znear() const;

    /**
     * @brief Access to the distance to the far clipping plane of the camera.
     *
     * @return A const reference to m_zfar.
     */
    const float& zfar() const;

    /**
     * @brief Access to the width of the displayed window handled by the camera.
     *
     * @return A const reference to m_width.
     */
    const int& width() const;

    /**
     * @brief Access to the height of the displayed window handled by the camera.
     *
     * @return A const reference to m_height.
     */
    const int& height() const;

    /**
     * @brief Compute the world position of the camera.
     *
     * The position of the camera is computed by inverting the view matrix
     * and extracting its last column.
     *
     * @return The world position of the camera.
     */
    glm::vec3 computePosition() const;

    /**
     * @brief Compute the world position of a given pixel.
     *
     * @param xCoord The x coordinate of the pixel.
     * @param yCoord The y coordinate of the pixel.
     * @return The world position of the pixel situated at (xCoord, yCoord).
     */
    glm::vec3 pixelToWorld(const float &xCoord, const float &yCoord) const;

    /**
     * @brief Compute a ray going from the position of camera to the world coordinate of a pixel.
     *
     * @param x The x coordinate of the pixel.
     * @param y The y coordinate of the pixel.
     * @return A ray whose origin is the position of the camera and direction points to the world position of the pixel situated at (x, y).
     */
    Ray computeRayThroughPixel(const float& x, const float& y);

private:
    int m_width; /*!< The width of the displayed window handle by the camera. */
    int m_height; /*!< The height of the displayed window handle by the camera. */
    float m_ratio; /*!< The ratio of the displayed window handle by the camera. */
    float m_fov; /*!< The field of view, also called vertical angle of the camera. */
    float m_znear; /*!< The distance to the near clipping plane of the camera. */
    float m_zfar; /*!< The distance to the far clipping plane of the camera. */
    glm::mat4 m_view; /*!< The view matrix of the camera. */
    glm:: mat4 m_projection; /*!< The projection matrix of the camera. */
};

/**
 * @brief Return a stream containing informations about an input camera.
 *
 * @param out A reference to an input stream.
 * @param camera A reference to a camera.
 * @return A stream with informations about the camera.
 */
std::ostream& operator << ( std::ostream& out, const Camera& camera );

#endif //CAMERA_HPP
