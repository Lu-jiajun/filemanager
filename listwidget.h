#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "mainwindow.h"

#include <QListWidget>
class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = Q_NULLPTR );
    virtual ~ListWidget();
    Ui::MainWindow *ui = MainWindow::mywidget->ui;
    void setuplable();
    QLabel *statusLabel;
    void freshlable();
    void sortbytype();
    void showsortresult();
    bool isCorrectNewName(QString newName);

    QStringList fmt;
    QStringList folds;
    int listwidgetcount =0;
public slots:
    void onListWIdget_doubleclicked(QListWidgetItem *item);
    void slotCustomContextMenu(const QPoint &point);
    void eraseItem();
    void openItem();
    void renameItem();
    void copyItem();
    void cutItem();

private:
    QStringList my_sortlist;
};
#endif // LISTWIDGET_H
