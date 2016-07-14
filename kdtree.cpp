#include "kdtree.h"
#include "vector"
#include "QFile"
#include "QDebug"
MyKDTree::MyKDTree()
{


}

MyKDTree::loadDate(QString filepath)
{
    QFile data(filepath);
    if(data.open(QFile::ReadOnly))
    {
        QTextStream in(&data);
        in.setCodec("UTF-8");
        QString line;
        //size_t n=1;
        while(in.readLineInto(&line))
        {
            QStringList trip=line.split("\t");
            kdtreeNode node(trip[0].toDouble(),trip[1].toDouble(),trip[2].toDouble(),trip[3].toInt());
            m_tree.insert(node);
            m_nodes.push_back(node);
        }

        data.close();
    }
    else
    {
        qDebug()<<"file missing";
    }
}

MyKDTree::showDate()
{
    for(int i=0;i<m_tree.size();i++)
    {
        qDebug()<<m_nodes[i].xyz[0]<<" "<<m_nodes[i].xyz[1]<<" "<<m_nodes[i].xyz[2];
    }
}

vector<int> MyKDTree::findRange(kdtreeNode s, double range)
{
    vector<kdtreeNode> withRange;
    vector<int> inrangeIndex;
    m_tree.find_within_range(s,range,back_inserter(withRange));

    for(int i=0;i<withRange.size();i++)
    {
        if(withRange[i].distance(s)<(range))
        {
            inrangeIndex.push_back(withRange[i].index);
        }
    }

    return inrangeIndex;
}

