#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include <filewidget.h>
//#include "filewidget.h"
#include "qurl.h"
#include <QFileSystemModel>
#include <QMainWindow>
#include <QShortcut>
#include <QAction>
#include <QProcess> //双击文件打开
QT_BEGIN_NAMESPACE
#include <QDesktopServices>
#include <qlabel.h>
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int treeviewclicledcount = 0;
    int treeviewclicledcount3 = 0;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int treeviewclicledcount2 = 0;
    void setupStatusBar();
    QLabel *statusLabel;
    void setupToolBar();
    void setupMenuBar();
    void about();
    void sortbytype();
//    void mytabremovetab();
//    void mytabaddtab();
    void updateButton();
    void setButton();

    void cdParent();
    void backChild();

//    void onPrevClicked();
//    void onNextClicked();

    QStringList walkon(QString path);
    //void addtab();
    static MainWindow *mywidget;
    //static MainWindow *mytreeview;
    Ui::MainWindow *ui;
    QStringList fileNames;
    //QStringList m_imgList;
    void showFiles(const QStringList &paths);
    //void testinsert();
    QWidget *add;
    QStringList historyPath;
//    int pathCount=0;


private slots:


    void on_treeView_2_clicked(const QModelIndex &index);
    void on_treeView_3_clicked(const QModelIndex &index);
    void on_treeView_3_double_clicked(const QModelIndex &index);
//    void onTreeViewDoubleClicked(const QModelIndex &index);
    void on_treeView_double_clicked(const QModelIndex &index);
    void on_treeView_clicked(const QModelIndex &index);
    void go();
    void back();

    void showsortresult();

    //void tablewidgetinit();

private:

    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    QFileSystemModel *filemodel2;
    QFileSystemModel *dirmodel_4;
    QStringList my_sortlist;
};


#endif // MAINWINDOW_H
