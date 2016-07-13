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
#include "QMessageBox"
#pragma execution_character_set("UTF-8")
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buildDict();

//setStyle()//给视图某列某项值进行上色
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(0,30);//index
    ui->tableView->setColumnWidth(1,40);//link

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

//    dictmodel.setHorizontalHeaderLabels(headers);
//    loadDictFile("dict.txt");
//    filtdictmodel.setSourceModel(&dictmodel);

    loadOriginDate("dict");
    loadTreefile();

    //loadfile
}

MainWindow::loadDictFile(QString filepath)
{
    QFile data(filepath);
    dictsize=0;
    if(data.open(QFile::ReadOnly))
    {
        QTextStream in(&data);
        in.setCodec("UTF-8");
        QString line;
        while(in.readLineInto(&line))
        {
            QStringList games=line.split("\t");
            for(int i=0;i<games.size();i++)
            {
                dictmodel.setItem(dictsize,i,new QStandardItem(games[i]));
            }
            dictsize++;
        }
    }
    else
    {
        qDebug()<<"file missing";
        exit(-1);
    }
}

MainWindow::loadOriginDate(QString filepath)
{
    QFile file(filepath);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString line;
        int orisize=1;
        while(in.readLineInto(&line))
        {
            origindata.insert(orisize,line);
            orisize++;
        }
        file.close();
    }
    else
    {
        qDebug()<<"file missing";
        exit(-1);
    }
}

MainWindow::loadTreefile()//读取文件并创建初赔、终赔、变化、PR四棵树
{
    avgtree_1.loadDate("avgtree1.txt");
    avgtree_2.loadDate("avgtree2.txt");
    avgchangetree.loadDate("avgchangetree.txt");
    PRtree.loadDate("PRtree.txt");
}

MainWindow::showCurrentModel(vector<int> indexs)
{
    for(int i=0;i<indexs.size();i++)
    {
        QString line=origindata[indexs[i]];
        QStringList games=line.split("\t");
        for(int j=0;j<games.size();j++)
        {
            currentModel.setItem(i,j,new QStandardItem(games[j]));
        }
    }
    ui->tableView->setModel(&currentModel);
    QMessageBox *box=new QMessageBox();
    box->setText(QString("%1个结果").arg(indexs.size()));
    box->show();
    statistic();
}

void MainWindow::on_pushButton_clicked()
{
    QString val=ui->lineEdit->text();
    QStringList vallist=val.split(" ");
    vector<int> temp=avgchangetree.findRange(kdtreeNode(vallist[0].toDouble(),vallist[1].toDouble(),vallist[2].toDouble(),999),ui->doubleSpinBox->value());
    if(temp.size()>0)
    {
        showCurrentModel(temp);
        statistic();
    }
    else
    {
        QMessageBox *box=new QMessageBox();
        box->setText("无结果");
        box->show();
    }
    //qDebug()<<vallist[0]<<vallist[1];
    }

MainWindow::statistic(QLabel *win,QLabel *draw,QLabel *lose)
{
    int result[4]={0,0,0,0};
    int handi[4]={0,0,0,0};
    for(int i=0;i<ui->tableView->model()->rowCount();i++)
    {
        QString c19,c20;
        c19=ui->tableView->model()->data(ui->tableView->model()->index(i,19)).toString();
        c20=ui->tableView->model()->data(ui->tableView->model()->index(i,20)).toString();
        if(c19=="主胜")
        {
            result[0]++;
        }
        else if(c19=="平局")
        {
            result[1]++;
        }
        else if(c19=="客胜")
        {
            result[2]++;
        }
        if(c20=="赢盘")
        {
            handi[0]++;
        }
        else if(c20=="走水")
        {
            handi[1]++;
        }
        else if(c20=="输盘")
        {
            handi[2]++;
        }
    }
    result[3]=result[0]+result[1]+result[2];
    handi[3]=handi[0]+handi[2];
    double oddspercent[3]={0,0,0};
    double handipercent[2]={0,0};
    oddspercent[0]=double(result[0])/double(result[3])*100;
    oddspercent[1]=double(result[1])/double(result[3])*100;
    oddspercent[2]=double(result[2])/double(result[3])*100;
    handipercent[0]=double(handi[0])/double(handi[3])*100;
    handipercent[1]=double(handi[1])/double(handi[3])*100;
    win->setText("|"+QString::number(oddspercent[0],'f',2)+"%|");
    draw->setText("|"+QString::number(oddspercent[1],'f',2)+"%|");
    lose->setText("|"+QString::number(oddspercent[2],'f',2)+"%|");


}
