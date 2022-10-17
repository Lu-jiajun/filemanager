#include "filewidget.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <qdirmodel.h>
#include <QFileDialog>
FileWidget::FileWidget(QWidget *widget)
{
    //name = tag;
    //tabWidgetInit();
    qDebug()<<"constructor of FileWidget";
    tabWidgetInit();
    //fileWidgetAddTab();
}
class PushButton : public QPushButton
{
public:
    QSize sizeHint() const override;

};
void FileWidget::fileWidgetAddTab()
{

    QWidget *newwidget = new QWidget;
    ui->tabWidget->addTab(newwidget,"newtab");
}

void FileWidget::tabWidgetInit()
{
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabPosition(QTabWidget::North);   // default
    ui->tabWidget->setTabShape(QTabWidget::Rounded);    // default
    //ui->tabWidget->setTabsClosable(true);
}






void FileWidget::on_listWidget_4_itemDoubleClicked(QListWidgetItem *item)
{

}

