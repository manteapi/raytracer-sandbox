#include "./../include/raytracer-sandbox/octree.hpp"

OctreeNode::~OctreeNode(){}

OctreeNode::OctreeNode()
{
    m_dataObject.clear();
    m_dataExtent = Extent();
    m_children.fill(nullptr);
    m_isLeaf = true;
}

std::vector<ObjectPtr>& OctreeNode::dataObject()
{
    return m_dataObject;
}

std::array<OctreeNodePtr,8>& OctreeNode::children()
{
    return m_children;
}

bool& OctreeNode::isLeaf()
{
    return m_isLeaf;
}

const bool& OctreeNode::isLeaf() const
{
    return m_isLeaf;
}

Octree::~Octree(){}

Octree::Octree(const Extent &extent)
{
    m_depth = 0;
    m_extent = extent;
    m_root = std::make_shared<OctreeNode>();
}

void Octree::insert(const ObjectPtr& o, OctreeNodePtr& node, std::array<glm::vec3, 2> nodeBB, int depth)
{
    if(node->isLeaf() == true)
    {
        //If the leaf is empty or the max depth of the octree has been reached
        //Just add the object to the leaf
        if(node->dataObject().empty() || depth>=m_depth)
        {
            node->dataObject().push_back(o);
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
        //Find the right node where to put the object
        int cellIndex = 0;
        OctreeNodePtr& child = node->children()[cellIndex];
        glm::vec3 nodeCentroid = 0.5f*(nodeBB[0]+nodeBB[1]);
        glm::vec3 oCentroid = 0.5f*(o->bbox().minExtent() + o->bbox().maxExtent());
        if(oCentroid[2]>nodeCentroid[2]) cellIndex += 4;
        if(oCentroid[1]>nodeCentroid[1]) cellIndex += 2;
        if(oCentroid[0]>nodeCentroid[0]) cellIndex += 1;

        //Compute the bounding box of the child node
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
            child = std::make_shared<OctreeNode>();
        }
        insert(o, child, childBB, depth+1);
    }
}
