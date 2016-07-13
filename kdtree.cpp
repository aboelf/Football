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
        QString line;
        size_t n=2;
        do
        {
            line=in.readLine();
            //qDebug()<<line<<endl;
            QStringList trip=line.split("\t");
            kdtreeNode node(trip[0].toDouble(),trip[1].toDouble(),trip[2].toDouble(),n);
            node.index=n;
            n++;
            m_tree.insert(node);
            m_nodes.push_back(node);
        } while(!line.isNull());

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
