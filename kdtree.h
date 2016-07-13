#ifndef KDTREE_H
#define KDTREE_H
#include<vector>
#include<string>
#include<kdtree.hpp>
#include <QString>
using namespace std;

struct kdtreeNode
{
    typedef double value_type;
    value_type xyz[3];
    size_t index;
    kdtreeNode(value_type a,value_type b,value_type c,size_t n)
    {
        xyz[0]=a;
        xyz[1]=b;
        xyz[2]=c;
        index=n;
    }

    double  operator[](size_t n) const
    {
        return xyz[n];
    }

    double distance(const kdtreeNode &node)
    {
        double x=xyz[0]-node.xyz[0];
        double y = xyz[1] - node.xyz[1];
        double z = xyz[2] - node.xyz[2];
        return (fabs(x)+fabs(y)+fabs(z));
    }
};
class MyKDTree
{
public:
    MyKDTree();
    loadDate(QString filepath);
    showDate();
private:
    KDTree::KDTree<3,kdtreeNode> m_tree;
    vector<kdtreeNode> m_nodes;
};

#endif // KDTREE_H
