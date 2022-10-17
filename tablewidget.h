#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

//#include <qtablewidget.h>
#include<QTableWidget>
#include "mainwindow.h"



class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = nullptr);
    //MyTableWidget(int rows, int columns, QWidget *parent = nullptr);
    void TableInit();
    Ui::MainWindow *ui = MainWindow::mywidget->ui;
    void contextMenu(const QPoint &pos);
    QLabel *statusLabel;
    void setuplable();
    void freshlable();
public slots:

    void openFile();
    void openinFolder();
    //void openinFolder();
    void ondoubleClick();
private:
    QFileSystemModel *filemodel2;

};

#endif // TABLEWIDGET_H
