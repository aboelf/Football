#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QString>
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

private:
    Ui::MainWindow *ui;
    QStandardItemModel dictmodel;
    QSortFilterProxyModel filtdictmodel;
};

#endif // MAINWINDOW_H
