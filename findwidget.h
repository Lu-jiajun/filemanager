#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QDockWidget>
#include <QWidget>
#include "mainwindow.h"


class FindWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit FindWidget(QWidget *parent = nullptr);
//, const QString &path = "", const QString &find = "*"
    Ui::MainWindow *ui = MainWindow::mywidget->ui;
    void browser();
    void find();
    void shouFiles();
    void showFiles(const QStringList &paths);
    void setFindInfo(const QStringList &paths);
    void setFindInfo(const QString &path, const QString &find);
    //void showsortresult();
    void animateFindClick();
    //void onCellActivated(int row, int column);
    void loadFindWidgetInfo();
    void saveFindWidgetInfo();
    void contextMenu(const QPoint &pos);
    int tablewidgetcount =0;

private:
    QStringList findFiles(const QStringList &files, const QString &text);
    QDir currentDir;

private slots:
    //void browse();
    //void find();
    void onCellActivated(int row, int column);

signals:
    void cellActivated(const QString &name);
};

#endif // FINDWIDGET_H
