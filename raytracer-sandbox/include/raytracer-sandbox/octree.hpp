#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <array>
#include <memory>
#include "extent.hpp"
#include "object.hpp"

/** @brief Class for the node of an octree
 *
 * About the indexing of the height children of a node.
 * Let denote by (x,y,z) the centroid of the node and by minus or plus the left/right/bottom/top side of the node.
 * Then the cells are indexed according to the following :
 * (-x,-y,-z) = 0
 * (-x,-y, z) = 1
 * (-x, y,-z) = 2
 * (-x, y, z) = 3
 * ( x,-y,-z) = 4
 * ( x,-y, z) = 5
 * ( x, y,-z) = 6
 * ( x, y, z) = 7
 */
class OctreeNode
{
public:
    typedef std::shared_ptr<OctreeNode> OctreeNodePtr;
    ~OctreeNode();
    OctreeNode();
    OctreeNode( const OctreeNode& node ) = default;
    const bool& isLeaf() const;
    bool& isLeaf();
    std::vector<ObjectPtr>& dataObject();
    std::array<OctreeNodePtr,8>& children();
private:
    std::array<OctreeNodePtr, 8> m_children;
    bool m_isLeaf;
    std::vector<ObjectPtr> m_dataObject;
};

typedef std::shared_ptr<OctreeNode> OctreeNodePtr;

class Octree
{
public:
    ~Octree();
    Octree(const Extent& extent, const int& depth);
    Octree(const Octree& octree) = default;
    void insert(const ObjectPtr& o);
    const int& depth();
    const Extent& extent();
    const OctreeNodePtr& root();
private:
    OctreeNodePtr m_root;
    Extent m_extent;
    int m_depth;
    void insert(const ObjectPtr& o, OctreeNodePtr& node, std::array<glm::vec3,2> nodeBB, int depth);
};

#endif // OCTREE_HPP
