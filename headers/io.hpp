#ifndef IO_HPP
#define IO_HPP

/** @file
 * @brief Input/Output functions.
 *
 * Currently, this file only contains I/O functions for OBJ meshes.
 * Some of the functions use external libraries.
*/

#include <vector>
#include <QVector2D>
#include <QVector3D>
#include <string>

/** @brief Collect mesh data from an OBJ file using tinyobj library.
 *
 * This function opens an OBJ mesh file to collect information such
 * as vertex position, vertex indices of a face, vertex normals and vertex
 * texture coordinates.
 *
 * @param filename The path to the mesh file.
 * @param positions The vertex positions.
 * @param indices The vertex indices of faces.
 * @param normals The vertex normals.
 * @param texcoords The vertex texture coordinates.
 * @return False if import failed, true otherwise.
 */
bool read_obj(
        const std::string& filename,
        std::vector<QVector3D>& positions,
        std::vector<unsigned int>& indices,
        std::vector<QVector3D>& normals,
        std::vector<QVector2D>& texcoords
        );

#endif //IO_HPP
