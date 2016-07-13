#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kdtree.h"
#include "QStringList"
#include "QSortFilterProxyModel"
#include "QFile"
#include "QTextStream"
#include "QList"
#include "QDebug"
#include "QStandardItem"
#pragma execution_character_set("UTF-8")
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buildDict();

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(0,30);//index
    ui->tableView->setColumnWidth(1,40);//link
//setStyle()//给视图某列某项值进行上色

}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::buildDict()
{
    QStringList headers;
    headers<<"index"<<"link"<<"game"<<"初盘上盘"<<"初盘"<<"初盘下盘"<<"终盘上盘"<<"终盘"<<"终盘下盘"<<"初盘主胜"<<"初盘和局"<<"初盘客胜"
          <<"终盘主胜"<<"终盘和局"<<"终盘客胜"<<"PR主胜"<<"PR和"<<"PR客胜"<<"结果"<<"比分"<<"盘口"<<"初奥主"<<"初澳和"<<"澳门客胜"<<"终澳主"<<"终澳和"<<"终澳客"
         <<"初威主"<<"初威和"<<"初威客"<<"终威主"<<"终威和"<<"终威客";

    dictmodel.setHorizontalHeaderLabels(headers);
    loadDictFile("dict_small");
    filtdictmodel.setSourceModel(&dictmodel);

/*test*/    ui->tableView->setModel(&filtdictmodel);
    //loadfile
}

MainWindow::loadDictFile(QString filepath)
{
    QFile data(filepath);
    if(data.open(QFile::ReadOnly))
    {
        QTextStream in(&data);
        in.setCodec("UTF-8");
        QString line;
        size_t n=0;
        do
        {
            line=in.readLine();
            //qDebug()<<line;
            QStringList games=line.split("\t");
            for(int i=0;i<games.size();i++)
            {
                dictmodel.setItem(n,i,new QStandardItem(games[i]));
            }
            n++;
        } while(!line.isNull());

    }
    else
    {
        qDebug()<<"file missing";
        exit(-1);
    }
}
