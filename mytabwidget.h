#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>
#include <myfilesytemmodel.h>
#include "mainwindow.h"

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    Ui::MainWindow *ui = MainWindow::mywidget->ui;
    explicit MyTabWidget(QWidget *parent = nullptr);

public slots:


};

#endif // MYTABWIDGET_H
