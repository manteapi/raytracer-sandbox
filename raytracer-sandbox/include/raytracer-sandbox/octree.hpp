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
template<typename TData>
class OctreeNode
{
public:
    typedef std::shared_ptr< OctreeNode<TData> > OctreeNodePtr;
    ~OctreeNode();
    OctreeNode();
    OctreeNode( const OctreeNode& node ) = default;
    const bool& isLeaf() const;
    bool& isLeaf();
    std::vector<TData>& dataObject();
    std::array<OctreeNodePtr,8>& children();
private:
    std::array<OctreeNodePtr, 8> m_children;
    bool m_isLeaf;
    std::vector<TData> m_dataObject;
};


template<typename TData>
class Octree
{
public:
    typedef std::shared_ptr< OctreeNode<TData> > OctreeNodePtr;
public:
    ~Octree();
    Octree(const Extent& extent, const int& maxDepth);
    Octree(const Octree& octree) = default;
    const int& maxDepth();
    const Extent& extent();
    const OctreeNodePtr& root();
    void insert(const std::pair<TData, glm::vec3>& o);
    int computeDepth();
private:
    OctreeNodePtr m_root;
    Extent m_extent;
    int m_maxDepth;
    void insert(const std::pair<TData, glm::vec3>& o, OctreeNodePtr& node, std::array<glm::vec3,2> nodeBB, int depth);
    void computeDepth(const OctreeNodePtr& node, int& depth);
};

#endif // OCTREE_HPP
