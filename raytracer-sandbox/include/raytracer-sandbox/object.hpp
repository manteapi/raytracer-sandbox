#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "material.hpp"
#include "ray.hpp"
#include "utils.hpp"
#include "box.hpp"

class Object
{
public:
    Object() = default;
    Object(const Object& object) = default;
    virtual ~Object();
    virtual bool Intersect(const Ray& r, glm::vec3& hitPosition, glm::vec3& hitNormal) = 0;
    MaterialPtr& material();
    const MaterialPtr& material() const;
    Box& bbox();
    const Box& bbox() const;
protected:
    MaterialPtr m_material;
    Box m_bbox;
};

typedef std::shared_ptr<Object> ObjectPtr;

#endif //OBJECT_HPP
