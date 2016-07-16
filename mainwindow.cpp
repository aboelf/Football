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
#include "mysortfilterproxymodel.h"
#include "algorithm"
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
    loadDictFile("dict");

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
    if(currentModel.rowCount()>0)
        currentModel.clear();
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

    statistic(ui->label_win,ui->label_draw,ui->label_lose);
}

MainWindow::getFOdds()
{
    if(!ui->lineEdit_fodds->text().isEmpty())
    {
        QStringList foddlist=ui->lineEdit_fodds->text().split("\t");
        fodds[0]=foddlist[0].toDouble();
        fodds[1]=foddlist[1].toDouble();
        fodds[2]=foddlist[2].toDouble();
    }
}

MainWindow::getLOdds()
{
    if(!ui->lineEdit_lodds->text().isEmpty())
    {
        QStringList loddlist=ui->lineEdit_lodds->text().split("\t");
        lodds[0]=loddlist[0].toDouble();
        lodds[1]=loddlist[1].toDouble();
        lodds[2]=loddlist[2].toDouble();
    }
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
    win->setText(QString::number(oddspercent[0],'f',2));
    draw->setText(QString::number(oddspercent[1],'f',2));
    lose->setText(QString::number(oddspercent[2],'f',2));


}

double MainWindow::BayesFormular(double x, double y, double z)
{
    return (x*y)/(x*y+z*(100-x))*100;
}

void MainWindow::on_pushButton_changeOdd_clicked()
{
    QString val=ui->lineEdit->text();
    QStringList vallist=val.split(" ");
    vector<int> temp=avgchangetree.findRange(kdtreeNode(vallist[0].toDouble(),vallist[1].toDouble(),vallist[2].toDouble(),999),ui->db_ChangeOdd->value());
    if(temp.size()>0)
    {
        showCurrentModel(temp);
    }
    else
    {
        QMessageBox *box=new QMessageBox();
        box->setText("无结果");
        box->show();
    }
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    if(arg1>0)
    {
        if(ui->comboBox->currentIndex()==0)
        {
            getFOdds();
            MySortFilterProxyModel *winoddfilterModel=new MySortFilterProxyModel(9);
            winoddfilterModel->setFilterMinimumValue(fodds[0]-ui->doubleSpinBox_2->value());
            winoddfilterModel->setFilterMaximumValue(fodds[0]+ui->doubleSpinBox_2->value());
            winoddfilterModel->setSourceModel(&currentModel);
            ui->tableView->setModel(winoddfilterModel);
            statistic(ui->label_win,ui->label_draw,ui->label_lose);
            double bwin=BayesFormular(ui->db_return->value()/fodds[0],ui->label_win->text().toDouble(),(100-ui->label_win->text().toDouble()));
            ui->label_bwin->setText(QString("%1").arg(bwin));
        }
        else
        {
            getLOdds();
            MySortFilterProxyModel *winoddfilterModel=new MySortFilterProxyModel(12);
            winoddfilterModel->setFilterMinimumValue(lodds[0]-ui->doubleSpinBox_2->value());
            winoddfilterModel->setFilterMaximumValue(lodds[0]+ui->doubleSpinBox_2->value());
            winoddfilterModel->setSourceModel(&currentModel);
            ui->tableView->setModel(winoddfilterModel);
            statistic(ui->label_win,ui->label_draw,ui->label_lose);
            double bwin=BayesFormular(ui->db_return->value()/lodds[0],ui->label_win->text().toDouble(),(100-ui->label_win->text().toDouble()));
            ui->label_bwin->setText(QString("%1").arg(bwin));
        }

    }
}

void MainWindow::on_db_return_valueChanged(double arg1)
{
    double bwin=BayesFormular(ui->db_return->value()/fodds[0],ui->label_win->text().toDouble(),(100-ui->label_win->text().toDouble()));
    ui->label_bwin->setText(QString("%1").arg(bwin));
}

void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    if(arg1>0)
    {
        if(ui->comboBox->currentIndex()==0)
        {
            getFOdds();
            MySortFilterProxyModel *loseoddfilterModel=new MySortFilterProxyModel(11);
            loseoddfilterModel->setFilterMinimumValue(fodds[2]-ui->doubleSpinBox_4->value());
            loseoddfilterModel->setFilterMaximumValue(fodds[2]+ui->doubleSpinBox_4->value());
            loseoddfilterModel->setSourceModel(&currentModel);
            ui->tableView->setModel(loseoddfilterModel);
            statistic(ui->label_win,ui->label_draw,ui->label_lose);
            double blose=BayesFormular(ui->db_return->value()/fodds[2],ui->label_lose->text().toDouble(),(100-ui->label_lose->text().toDouble()));
            ui->label_blose->setText(QString("%1").arg(blose));
        }
        else
        {
            getLOdds();
            MySortFilterProxyModel *loseoddfilterModel=new MySortFilterProxyModel(14);
            loseoddfilterModel->setFilterMinimumValue(lodds[2]-ui->doubleSpinBox_4->value());
            loseoddfilterModel->setFilterMaximumValue(lodds[2]+ui->doubleSpinBox_4->value());
            loseoddfilterModel->setSourceModel(&currentModel);
            ui->tableView->setModel(loseoddfilterModel);
            statistic(ui->label_win,ui->label_draw,ui->label_lose);
            double blose=BayesFormular(ui->db_return->value()/lodds[2],ui->label_lose->text().toDouble(),(100-ui->label_lose->text().toDouble()));
            ui->label_blose->setText(QString("%1").arg(blose));
        }

    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_changeOdd_clicked();
}

void MainWindow::on_pushButton_clicked()
{
    QString val=ui->lineEdit_PR->text();
    QStringList vallist=val.split("\t");
    vector<int> temp=PRtree.findRange(kdtreeNode(vallist[0].toDouble(),vallist[1].toDouble(),vallist[2].toDouble(),999),ui->db_PRrange->value());
    if(temp.size()>0)
    {
        showCurrentModel(temp);
    }
    else
    {
        QMessageBox *box=new QMessageBox();
        box->setText("无结果");
        box->show();
    }
}

void MainWindow::on_pushButton_2_clicked()//ODDCHANGEPR testfunction
{
    QString val1=ui->lineEdit->text();
    QStringList val1list=val1.split(" ");
    vector<int> temp1=avgchangetree.findRange(kdtreeNode(val1list[0].toDouble(),val1list[1].toDouble(),val1list[2].toDouble(),999),ui->db_ChangeOdd->value());
    QString val2=ui->lineEdit_PR->text();
    QStringList val2list=val2.split("\t");
    vector<int> temp2=PRtree.findRange(kdtreeNode(val2list[0].toDouble(),val2list[1].toDouble(),val2list[2].toDouble(),999),ui->db_PRrange->value());

    if(temp1.size()>0&&temp2.size()>0)
    {
        vector<int> v;
        sort(temp1.begin(),temp1.end());
        sort(temp2.begin(),temp2.end());
        set_intersection(temp1.begin(),temp1.end(),temp2.begin(),temp2.end(),std::back_inserter(v));
        if(v.size()>0)
            showCurrentModel(v);
        else
        {
            QMessageBox *box=new QMessageBox();
            box->setText("无结果");
            box->show();
        }
    }
}


