#include "./../include/raytracer-sandbox/aabbtree.hpp"

using namespace std;

Node::~Node()
{}

Node::Node(const Box &aabb, const int &data)
{
    m_aabb = aabb;
    m_data = data;
    m_children = {{nullptr, nullptr}};
}

NodePtr& Node::left()
{
    return m_children[0];
}

const NodePtr& Node::left() const
{
    return m_children[0];
}

NodePtr& Node::right()
{
    return m_children[1];
}

const NodePtr& Node::right() const
{
    return m_children[1];
}

const int& Node::data()
{
    return m_data;
}

const Box& Node::aabb()
{
    return m_aabb;
}

AABBTree::~AABBTree()
{}

void AABBTree::insert(const Box& aabb, const int& data)
{
    if(m_root == nullptr)
    {
        //Left and right children are set to null_ptr in the constructor
        m_root = std::make_shared<Node>(aabb, data);
    }
    else
    {
        //TODO
    }
}

bool AABBTree::intersect(const Ray& r, int& data)
{
    //TODO
    std::array<float, 2> t;
    if( Intersect(r, m_root->aabb(), t) )
    {
        if(t[0]>0 || t[1] >0)
        {
            data = m_root->data();
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}
