#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <qtabwidget.h>
#include "mainwindow.h"

class FileWidget :public QListWidget
{
    QT_END_NAMESPACE
public:
    QString name;
    Ui::MainWindow *ui = MainWindow::mywidget->ui;
    explicit FileWidget(QWidget *parent = nullptr);
    void fileWidgetAddTab();//添加tabwidget
    void tabWidgetInit();

private slots:
    void on_pushButton_clicked();
    void on_listWidget_4_itemDoubleClicked(QListWidgetItem *item);

private:

};

#endif // FILEWIDGET_H
