#ifndef TREEVIEW_H
#define TREEVIEW_H
#include <QMenu>
#include <qtreeview.h>
#include <QTimer>
#include "mainwindow.h"

class TreeView : public QTreeView
{
    Q_OBJECT //使用信号与槽所必需的
public:
    explicit TreeView(QWidget *parent = nullptr);
    Ui::MainWindow *ui = MainWindow::mywidget->ui;
    void warningMessageBox(QModelIndex &index,QFileSystemModel *model);
//    void openF(QString fileName,QAction *open);
    QFileSystemModel filemodel;
    void open(QString fileName);


    void setuplable(QString path);
    QLabel *statusLabel;
    void freshlable(QString path);
    friend MainWindow;
    QString ifSameName(QString _SnapPath,QDir *snapFile,char i);
   // QString ifSameName(QString _SnapPath,QDir *snapFile,char i);

public slots:
    void slotCustomContextMenu(const QPoint &point);//创建右键菜单的槽函数
    void openFile();
    void deleteFile();
    void renameFile();
    void copy();
    void copyToClipboard();
    bool copyFile();
    void onTreeViewDoubleClicked(const QModelIndex &index, QFileSystemModel *model);
    bool isCorrectNewName(QString newName);
    void newFolder();
    void cut();
    QString on_treeView_clicked(const QModelIndex &index);
private:
    //QFileSystemModel *filemodel2;


};

#endif // TREEVIEW_H
