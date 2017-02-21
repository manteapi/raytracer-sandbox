#ifndef RAY_HPP
#define RAY_HPP

/** @file
 *  @brief Define a ray.
 *
 * This file defines the Ray class that implements the basics of a ray.
 */

#include <glm/glm.hpp>
#include <array>
#include <iostream>

/**
 * \brief Define a ray which starts from an origin and points to a direction.
 *
 * The Ray class describes a ray. A ray is points to a direction from an fixed position which defines its origin.
 */
class Ray
{
public:
    /** @brief Destructor
     *
     * Instance destructor.
    */
    ~Ray();

    /** @brief Default constructor
     *
     * Default constructor.
    */
    Ray() = delete;

    /** @brief Clone constructor
     *
     * Clone constructor.
    */
    Ray(const Ray& r) = default;

    /** @brief Constructor with ray origin and direction.
     *
     * Construct a new ray from an origin and a direction.
     * \param origin The origin of the ray.
     * \param direction The direction of the ray.
     */
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    /** @brief Read/write accessor to the direction of the ray.
     *
     * Allow to read and write the direction of the ray.
     * @return A reference to the direction of the ray.
     */
    glm::vec3& direction();

    /** @brief Read-only accessor to the direction of the ray.
     *
     * Allow to read the direction of the ray.
     * @return A const reference to the direction of the ray.
     */
    const glm::vec3& direction() const;

    /** @brief Read-only accessor to the inverse of the direction of the ray.
     *
     * Allow to read the direction of the ray.
     * @return A const reference to the direction of the ray.
     */
    const glm::vec3& invDirection() const;

    /** @brief Read-only accessor to the sign of the direction of the ray.
     *
     * Allow to read the direction of the ray.
     * @return A const reference to the direction of the ray.
     */
    const std::array<int,3>& sign() const;

    /** @brief Read/write accessor to the origin of the ray.
     *
     * Allow to read and write the origin of the ray.
     * @return A reference to the origin of the ray.
     */
    glm::vec3& origin();

    /** @brief Read-only accessor to the origin of the ray.
     *
     * Allow to read the origin of the ray.
     * @return A const reference to the origin of the ray.
     */
    const glm::vec3& origin() const;

private:
    glm::vec3 m_direction; /*!< The direction of the ray. */
    glm::vec3 m_invDirection; /*!< The inverse of the direction of the ray.*/
    std::array<int,3> m_sign; /*!< The sign of the ray direction.*/
    glm::vec3 m_origin; /*!< The origin of the ray. */
};

/*! \fn std::ostream& operator << ( std::ostream& out, const Ray& ray)
    \brief Return a stream containing human readable informations about the input ray.
    \param out The input stream.
    \param ray A ray.
    \return A stream with human readable informations about the input ray.
*/
std::ostream& operator << ( std::ostream& out, const Ray& ray);

#endif //RAY_HPP
