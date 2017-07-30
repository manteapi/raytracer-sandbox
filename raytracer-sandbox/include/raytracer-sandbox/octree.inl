#ifndef OCTREE_INL
#define OCTREE_INL

#include "octree.hpp"

template<typename TData>
OctreeNode<TData>::~OctreeNode(){}

template<typename TData>
OctreeNode<TData>::OctreeNode()
{
    m_dataObject.clear();
    m_children.fill(nullptr);
    m_isLeaf = true;
    m_extent = Extent();
    m_center = glm::vec3(0,0,0);
}

template<typename TData>
OctreeNode<TData>::OctreeNode(const Extent& extent)
{
    m_extent = extent;
    m_children.fill(nullptr);
    m_isLeaf = true;
    std::array<glm::vec3,2> bounds = m_extent.bounds();
    m_center = 0.5f*(bounds[0] + bounds[1]);
}

template<typename TData>
const std::vector< std::pair<TData,glm::vec3> >  &OctreeNode<TData>::dataObject() const
{
    return m_dataObject;
}

template<typename TData>
std::vector< std::pair<TData,glm::vec3> >  &OctreeNode<TData>::dataObject()
{
    return m_dataObject;
}

template<typename TData>
const std::array< std::shared_ptr< OctreeNode<TData> >,8>& OctreeNode<TData>::children() const
{
    return m_children;
}

template<typename TData>
std::array< std::shared_ptr< OctreeNode<TData> >,8>& OctreeNode<TData>::children()
{
    return m_children;
}

template<typename TData>
bool& OctreeNode<TData>::isLeaf()
{
    return m_isLeaf;
}

template<typename TData>
const bool& OctreeNode<TData>::isLeaf() const
{
    return m_isLeaf;
}

template<typename TData>
const glm::vec3& OctreeNode<TData>::center() const
{
    return m_center;
}

template<typename TData>
glm::vec3& OctreeNode<TData>::center()
{
    return m_center;
}

template<typename TData>
const Extent& OctreeNode<TData>::extent() const
{
    return m_extent;
}

template<typename TData>
Extent& OctreeNode<TData>::extent()
{
    return m_extent;
}

template<typename TData>
Octree<TData>::~Octree(){}

template<typename TData>
Octree<TData>::Octree(const Extent &extent, const int& maxDepth) : m_extent(extent), m_maxDepth(maxDepth)
{
    m_root = std::make_shared< OctreeNode<TData> >();
}

template<typename TData>
const int& Octree<TData>::maxDepth()
{
    return m_maxDepth;
}

template<typename TData>
const Extent& Octree<TData>::extent()
{
    return m_extent;
}

template<typename TData>
const std::shared_ptr< OctreeNode<TData> >& Octree<TData>::root()
{
    return m_root;
}

template<typename TData>
void Octree<TData>::insert(const std::pair<TData, glm::vec3>& o)
{
    insert(o, m_root, m_extent.bounds(), 0);
}

template<typename TData>
int Octree<TData>::computeDepth()
{
    int depth=-1;
    computeDepth(m_root, depth);
    return depth;
}

template<typename TData>
void Octree<TData>::insert(const std::pair<TData, glm::vec3>& o, OctreeNodePtr& node, std::array<glm::vec3, 2> nodeBB, int depth)
{
    if(node->isLeaf() == true)
    {
        //If the leaf is empty or the max depth of the octree has been reached
        //Just add the object to the leaf
        if(node->dataObject().empty() || depth>=m_maxDepth)
        {
            node->dataObject().push_back(o.first);
        }
        else
        {
            //Mark the leaf as a branch
            node->isLeaf() = false;
            //Empty the content of the leaf and re-insert in the octree from this node
            std::vector<ObjectPtr>& nodeObjects = node->dataObject();
            while(!nodeObjects.empty())
            {
                insert(nodeObjects.back(), node, nodeBB, depth);
                nodeObjects.pop_back();
            }
            //Also Insert the current object from this node.
            insert(o, node, nodeBB, depth);
        }
    }
    else //The node is a branch
    {
        //Between the 8 nodes, find the right one that contains the centroid of the object
        int cellIndex = 0;
        glm::vec3 nodeCentroid = 0.5f*(nodeBB[0]+nodeBB[1]);
        glm::vec3 oCentroid = o.second;
        if(oCentroid[2]>nodeCentroid[2]) cellIndex += 4;
        if(oCentroid[1]>nodeCentroid[1]) cellIndex += 2;
        if(oCentroid[0]>nodeCentroid[0]) cellIndex += 1;
        OctreeNodePtr& child = node->children()[cellIndex];

        //Compute the bounding box of the child node:
        //  For each axis, for mininmum and maximum bound,
        //      1- Compare the centroid of the object with the centroid of the node
        //      2- Choose between the coordinate of the node centroid and the coordinate of th node bounding box.
        std::array<glm::vec3,2> childBB;
        childBB[0][0] =  oCentroid[0]>nodeCentroid[0] ? nodeCentroid[0] : nodeBB[0][0];
        childBB[1][0] =  oCentroid[0]>nodeCentroid[0] ? nodeBB[1][0] : nodeCentroid[0];
        childBB[0][1] =  oCentroid[1]>nodeCentroid[1] ? nodeCentroid[1] : nodeBB[0][1];
        childBB[1][1] =  oCentroid[1]>nodeCentroid[1] ? nodeBB[1][1] : nodeCentroid[1];
        childBB[0][2] =  oCentroid[2]>nodeCentroid[2] ? nodeCentroid[2] : nodeBB[0][2];
        childBB[1][2] =  oCentroid[2]>nodeCentroid[2] ? nodeBB[1][2] : nodeCentroid[2];

        //Add the object to the child node
        if(child == nullptr)
        {
            //If the cell was not yet instanciated (optimization purpose) then create the cell
            child = std::make_shared<OctreeNode<TData> >( Extent(childBB) );
        }
        insert(o, child, childBB, depth+1);
    }
}

template<typename TData>
void Octree<TData>::computeDepth(const OctreeNodePtr& node, int& depth)
{
    if(node != nullptr)
    {
        std::array<OctreeNodePtr,8> children = node->children();
        bool hasChildren = false;
        for(size_t i=0; i<children.size(); ++i)
        {
            if(children[i]!=nullptr)
            {
                hasChildren = true,
                computeDepth(children[i], depth);
            }
        }
        depth += hasChildren ? 1 : 0;
    }
    else
    {
        depth=-1;
    }
}



#endif
