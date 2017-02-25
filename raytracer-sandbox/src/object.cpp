#include "./../include/raytracer-sandbox/object.hpp"
#include "./../include/raytracer-sandbox/io.hpp"
#include <iostream>
#include <assert.h>
#include <random>

using namespace std;

Object::~Object(){}

MaterialPtr &Object::material()
{
    return m_material;
}

const MaterialPtr &Object::material() const
{
    return m_material;
}

Box& Object::bbox()
{
    return m_bbox;
}

const Box& Object::bbox() const
{
    return m_bbox;
}
