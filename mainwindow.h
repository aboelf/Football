#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QString>
#include <QMap>
#include <kdtree.h>
#include <QLabel>
#include <mysortfilterproxymodel.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    buildDict();
    loadDictFile(QString filepath);
    loadOriginDate(QString filepath);//读取原始文件
    loadTreefile();//读取文件并创建初赔、终赔、变化、PR四棵树
    showCurrentModel(vector<int> indexs);

    getFOdds();//获得初赔
    getLOdds();
private slots:
    void on_pushButton_changeOdd_clicked();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_db_return_valueChanged(double arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    statistic(QLabel *win, QLabel *draw, QLabel *lose);

    double BayesFormular(double x,double y,double z);
    Ui::MainWindow *ui;
    QStandardItemModel dictmodel;
    QStandardItemModel currentModel;
    int dictsize;

    QMap<size_t,QString> origindata;

    MyKDTree avgtree_1;
    MyKDTree avgtree_2;
    MyKDTree avgchangetree;
    MyKDTree PRtree;

    double fodds[3];
    double lodds[3];

};

#endif // MAINWINDOW_H
