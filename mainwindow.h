#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QString>
#include <QMap>
#include <kdtree.h>
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
private slots:
    void on_pushButton_clicked();

private:
    statistic(QLabel *win, QLabel *draw, QLabel *lose);

    Ui::MainWindow *ui;
    QStandardItemModel dictmodel;
    QStandardItemModel currentModel;
    int dictsize;
    QSortFilterProxyModel filtdictmodel;
    QMap<size_t,QString> origindata;

    MyKDTree avgtree_1;
    MyKDTree avgtree_2;
    MyKDTree avgchangetree;
    MyKDTree PRtree;


};

#endif // MAINWINDOW_H
