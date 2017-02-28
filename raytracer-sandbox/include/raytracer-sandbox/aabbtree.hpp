#ifndef TREE_HPP
#define TREE_HPP

#include "box.hpp"
#include <memory>

class Node
{
    typedef std::shared_ptr< Node > NodePtr;
public:
    ~Node();
    Node() = default;
    Node(const Node& node) = default;
    Node(const Box& aabb, const int& data);
    NodePtr& left();
    const NodePtr& left() const;
    NodePtr& right();
    const NodePtr& right() const;
    const int& data();
    const Box& aabb();
private:
    int m_data; //Triangle id, only for leaves, -1 otherwise
    Box m_aabb; //Fat AABB for leaves, bounding AABB for branches
    std::array<NodePtr,2> m_children; //Only for branches, null_ptr otherwise
};

typedef std::shared_ptr< Node > NodePtr;

class AABBTree
{
public:
    ~AABBTree();
    AABBTree() = default;
    AABBTree(const AABBTree& tree) = default;
    void insert(const Box& aabb, const int& data);
    bool intersect(const Ray& r, int &data);
private:
    NodePtr m_root;
};

#endif // TREE_HPP
