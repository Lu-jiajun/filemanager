#include "tablewidget.h"
#include "qdebug.h"
#include "ui_mainwindow.h"
#include <QMenu>

TableWidget::TableWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    TableInit();
    connect(this,&TableWidget::itemDoubleClicked,this,&TableWidget::ondoubleClick);

}
QString fileName;
void TableWidget::TableInit(){
    setColumnCount(2);//只设置列数，行数动态中增加
//    setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("filename")
//                                                             <<QString::fromUtf8("filesize"));
//    //setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
//    setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
//    setSelectionMode(QAbstractItemView::SingleSelection);//可以选中单个
    QStringList labels;
    labels << tr("Filename") << tr("Size");
    setHorizontalHeaderLabels(labels);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize((30));
    setShowGrid(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &::TableWidget::customContextMenuRequested,
            this, &TableWidget::contextMenu);
//    connect(this, &QTableWidget::cellActivated,
//            this, &::TableWidget::cellActivated);

}
enum { absoluteFileNameRole = Qt::UserRole + 1 };
static inline QString fileNameOfItem(const QTableWidgetItem *item)
{
    return item->data(absoluteFileNameRole).toString();
}

void TableWidget::contextMenu(const QPoint &pos)
{
    const QTableWidgetItem *item = this->itemAt(pos);
    if (!item)
        return;
//    QMenu menu;
    fileName = this->itemAt(pos)->data(absoluteFileNameRole).toString();
    qDebug()<<"<<<<<<<<"<<fileName;
    QPoint globalPos = this->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction(QIcon(":/resource/openFile.png"),"open",  this, SLOT(openFile()));
    myMenu.addAction(QIcon(":/resource/openContainingFolder.png"),tr("Open Containing &Folder"), this, SLOT(openinFolder()));






    // Show context menu at handling position
    myMenu.exec(globalPos);



}
void TableWidget::openFile()
{
    qDebug()<<"??????"<<QFileInfo(fileName).absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}


void TableWidget::openinFolder(){

    qDebug()<<"go";
    QString path=QFileInfo(fileName).absolutePath();
    filemodel2=new QFileSystemModel();
    filemodel2->setRootPath(path);
    ui->treeView->setModel(filemodel2);
    //    m_treeview->setRootIndex(m_model->index(currPath));
    QFileInfo info(path);
    if(info.isDir())
    {
        ui->treeView->setRootIndex(filemodel2->index(path));
        ui->tabWidget->setCurrentIndex(1);
        ui->lineEdit_3->setText(path);
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }

}



void TableWidget::ondoubleClick(){
    QString clickchange = ui->tableWidget->currentItem()->data(absoluteFileNameRole).toString();
    QFileInfo thepath(clickchange);
    filemodel2=new QFileSystemModel();
    filemodel2->setRootPath(clickchange);
    ui->treeView->setModel(filemodel2);
    if(thepath.isDir()){
        ui->treeView->setRootIndex(filemodel2->index(clickchange));
        ui->tabWidget->setCurrentIndex(1);
        ui->lineEdit_3->setText(clickchange);
    }else{
        QDesktopServices::openUrl(QUrl::fromLocalFile(clickchange));
    }


}
void  TableWidget::setuplable(){
    statusLabel=new QLabel(this);
    int i = this->rowCount();
    QString text = "there is_"+QString::number(i)+"_files finded";
    qDebug()<<"there is "<<i<<"row in the find widget";
    statusLabel->setText(text);
    ui->statusBar->removeWidget(ui->treeView_3->statusLabel);
    ui->statusBar->addWidget(statusLabel);



}

void TableWidget::freshlable(){
    ui->statusBar->removeWidget(statusLabel);
    setuplable();
    //ui->statusBar->addWidget(statusLabel);

}
