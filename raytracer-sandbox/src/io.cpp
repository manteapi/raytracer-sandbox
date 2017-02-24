#include "./../include/raytracer-sandbox/io.hpp"
#include <iostream>
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tiny_obj_loader.h>

using namespace std;

bool read_obj(const string& filename,
        vector<glm::vec3>& positions,
        vector<unsigned int>& triangles,
        vector<glm::vec3>& normals,
        vector<glm::vec2>& texcoords
        )
{
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string err;

    bool ret = tinyobj::LoadObj(shapes, materials, err, filename.c_str());

    if (!err.empty())
    {
        cerr << err << endl;
    }

    if (!ret)
    {
        return ret;
    }

    positions.clear();
    triangles.clear();
    normals.clear();
    texcoords.clear();

    for (size_t i = 0; i < shapes.size(); i++)
    {
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
        {
            triangles.push_back( shapes[i].mesh.indices[f] );
        }
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++)
        {
            positions.push_back( glm::vec3(shapes[i].mesh.positions[3*v+0], shapes[i].mesh.positions[3*v+1],shapes[i].mesh.positions[3*v+2]) );
        }

        assert((shapes[i].mesh.normals.size() % 3) == 0);
        for (size_t n = 0; n < shapes[i].mesh.normals.size() / 3; n++)
        {
            normals.push_back( glm::vec3(shapes[i].mesh.normals[3*n+0], shapes[i].mesh.normals[3*n+1],shapes[i].mesh.normals[3*n+2]) );
        }

        assert((shapes[i].mesh.texcoords.size() % 2) == 0);
        for (size_t t = 0; t < shapes[i].mesh.texcoords.size() / 2; t++)
        {
            texcoords.push_back( glm::vec2(shapes[i].mesh.texcoords[2*t+0], shapes[i].mesh.texcoords[2*t+1]) );
        }
    }

    return ret;
}
