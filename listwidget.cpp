#include "listwidget.h"
#include <myfilesytemmodel.h>
#include "qdebug.h"
#include <QListWidget>
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

//    int treeviewclicledcount = 0;
//    int treeviewclicledcount2= 0;
//    int treeviewclicledcount3 = 0;
//    int tablewidgetcount =0;
//    int listwidgetcount = 0;

ListWidget::ListWidget(QWidget *parent) : QListWidget()

{

//    this->setSelectionMode(QAbstractItemView::MultiSelection);
    qDebug()<<"ListItem starting work";
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this, SLOT(onListWIdget_doubleclicked(QListWidgetItem *)));
    //connect(ui->listWidget_4,&QListWidget::itemDoubleClicked,this,ListWidget::onListWIdget_doubleclicked);
    Q_UNUSED(parent)

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    //qDebug()<<"TreeView working";

    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(slotCustomContextMenu(const QPoint &)));
//    connect(ui->comboBox,&QComboBox::currentTextChanged,this,&ListWidget::sortbytype);

}
static  void openFile(const QString &fileName)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}
void ListWidget::onListWIdget_doubleclicked(QListWidgetItem *item){
    qDebug()<<"ListItemDoubleClicked";
    QString path;
    //path = ui->lineEdit->text();
    path = ui->listWidget_4->currentItem()->text();
    qDebug()<<"path is  ready"<<path;
    openFile(path);

}

void ListWidget::slotCustomContextMenu(const QPoint &point) //槽函数定义
{
    QPoint globalPos = this->mapToGlobal(point);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction(QIcon(":/resource/openFile.png"),"open",  this, SLOT(openItem()));
    myMenu.addAction(QIcon(":/resource/delete.png"),"delete",  this, SLOT(eraseItem()));
    myMenu.addAction(QIcon(":/resource/rename.png"),"rename", this, SLOT(renameItem()));
    myMenu.addAction(QIcon(":/resource/cut.png"),"cut", this, SLOT(cutItem()));//将选中文件移动到路径
    myMenu.addAction(QIcon(":/resource/copy.png"),"copy", this, SLOT(copyItem()));//将选中文件复制到路径

//    myMenu.addAction("copy", this, SLOT(addItem()));



    // Show context menu at handling position
    myMenu.exec(globalPos);

}


void ListWidget::openItem(){
    openFile(ui->listWidget_4->currentItem()->text());

}
void ListWidget::eraseItem()
{
    statusLabel->clear();
    // If multiple selection is on, we need to erase all selected items
    for (int i = 0; i < this->selectedItems().size(); ++i) {
        // Get curent item on selected row
        QListWidgetItem *item = ui->listWidget_4->takeItem(this->currentRow());
        qDebug()<<ui->listWidget_4->currentItem()->text();
        // And remove it
        QString filepath = item->text();
        delete item;



        QFile file(filepath);
        qDebug()<<filepath;
        file.remove();
        freshlable();


        }

    }

void ListWidget::renameItem()
{
    int index=this->currentRow();

    QString oldName=folds[index];
    QString old=oldName;
    qDebug()<<"oldName="<<oldName;
    qDebug()<<"index="<<index;
    QFileInfo *file=new QFileInfo(oldName);
    qDebug()<<"baseName"<<file->baseName();
//    QString fileSuffix=file->suffix();
    QString fileName=file->baseName()+"."+file->suffix();
    QString oldFileName=fileName;
    qDebug()<<"fileName="<<fileName;

    bool ok=0;
    QString newName=QInputDialog::getText(this,tr("Rename file"),tr("Please enter file's new name:"),QLineEdit::Normal,"",&ok);
    qDebug()<<ok;
    if(ok)
    {
        if(newName.isEmpty())
        {
            QMessageBox::critical(this,"Rename file","Invalid file name",QMessageBox::Yes);
        }
        else
        {
            if(isCorrectNewName(newName))
            {
                QString tempName=fileName.replace(file->baseName(),newName);
                QString finallNewName=oldName.replace(oldFileName,tempName);
                QDir dir;
                qDebug()<<dir.rename(old,finallNewName);
                qDebug()<<"oldName"<<"finallNewName"<<oldName<<finallNewName;
//                folds[index]=finallNewName;

                sortbytype();
                showsortresult();
                qDebug()<<"fileName="<<fileName;
                qDebug()<<"tempName="<<tempName;
                qDebug()<<"finallNewName="<<finallNewName;
            }
            else
            {
                QMessageBox::critical(this,"Rename file","Invalid file name",QMessageBox::Yes);
            }
        }
    }

}

bool ListWidget::isCorrectNewName(QString newName)
{
    if(newName.contains("/")||newName.contains("\\")||newName.contains(":")||newName.contains("*")||newName.contains("?")||newName.contains("<")||newName.contains(">")||newName.contains("|"))
    {
        return false;
    }
    return true;
}

void ListWidget::copyItem()
{
    int index=this->currentRow();
    QString oldName=folds[index];
    QString path;
    path = ui->listWidget_4->currentItem()->text();
    QString dstFolderPath=QFileDialog::getExistingDirectory(this,"Select a folder to paste",path);
//    QFile currentFile(oldName);

    QDir *createfile  = new QDir;
    bool exist = createfile->exists(dstFolderPath);
    if (exist){
        createfile->remove(dstFolderPath);
    }

    QFileInfo *file=new QFileInfo(oldName);
    QString fileName=file->baseName()+"."+file->suffix();
    QFileInfo info(fileName);
    dstFolderPath=dstFolderPath+'/'+fileName;
    QFile currentFile(oldName);
    QFileInfo selectPath(oldName);
    QString thePath = selectPath.absoluteDir().absolutePath();
    qDebug()<<selectPath.absoluteFilePath()<<"==vsvsvsvs==\n"<<dstFolderPath;
    if(selectPath.absoluteFilePath()!=dstFolderPath){
        qDebug()<<"进入if";
        currentFile.copy(dstFolderPath);}
    else {
        int count = 0;
        qDebug()<<"ssshhh89998";
        QString newName;//= thePath+"/"+file->baseName()+QString::number(count)+"."+file->suffix();
        newName = thePath+"/"+file->baseName()+QString::number(count)+"."+file->suffix();
        QFileInfo newInfo(newName);
        do{
            count++;
            newName = thePath+"/"+file->baseName()+QString::number(count)+"."+file->suffix();
            QFileInfo newInfo1(newName);
            newInfo = newInfo1;
        }while(newInfo.isFile());
        qDebug()<<"被复制文件的新名字"<<newName;
        QFile::copy(oldName,newName);
    }
    sortbytype();
}


void ListWidget::cutItem()
{
    copyItem();
    eraseItem();
}

ListWidget::~ListWidget(){

}
void ListWidget::freshlable(){
    ui->statusBar->removeWidget(statusLabel);
    ui->statusBar->addWidget(statusLabel);
    setuplable();
}
void  ListWidget::setuplable(){
    statusLabel=new QLabel(this);
    int i = this->count();
    qDebug()<<i;
    QString text = "_"+QString::number(i)+"_files founded";
    //QString tt = "---";
    statusLabel->setText(text);
    //ui->listWidget_4->statusLabel->se
    QWidget *statuwidget;
    ui->statusBar->addWidget(statusLabel);
    //statuwidget = ui->statusBar->addWidget(statusLabel);；

    /*tatusTip();
    statusTip().fill(i);*/
    //statusTip().clear();
    //statusTip().swap(text);
    //statusTip().append(text);
    qDebug()<<i;
}

void walkin(const QString& foldPath,QStringList &folds,const QStringList& formats) {

    QDirIterator it(foldPath,QDir::Files | QDir::NoDotAndDotDot ,QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        if(formats.contains(fileInfo.suffix())) { //检测格式，按需保存
            folds << fileInfo.absoluteFilePath();
        }
    }
}

void ListWidget::sortbytype(){

    this->clear();
    folds.clear();
    int i = ui->comboBox->currentIndex();
    QString string=ui->comboBox->lineEdit()->text();
    //qDebug()<<string;
    qDebug()<<ui->lineEdit->text();
    QString sortpath=ui->lineEdit->text();
    auto model = new QFileSystemModel(ui->treeView);
    QString filter_index = "*."+ui->comboBox->currentText();

    QDir dir(sortpath);
    QDir dir_file(sortpath);
    dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks );
    if (!dir.exists()) {
        qDebug()<<sortpath + " ---不存在--- "<<endl;
            return;
    }
    // 设置过滤器
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << filter_index;
    //filters << "*.png";
    dir.setNameFilters(filters);
    QString dirpath =ui->lineEdit->text()+"/";
//    QStringList folds;
    this->clear();
    //QStringList fmt = QString("jpg;png").split(';');
//    QStringList fmt;
    fmt.clear();
    fmt.append(string);
    qDebug() <<"fmt"<< fmt;
    walkin(dirpath,folds,fmt);
    qDebug() << "folds"<<folds;
    my_sortlist = folds;
    //this->addItems(folds);
    //m_imgList = dir.entryList()
    //ui->listWidget_4->clear();
    showsortresult();
    //ui->statusBar->removeWidget(statusLabel);
    //ui->statusBar->addWidget(ui->listWidget_4->statusLabel);
   // ui->statusBar->addWidget(statusLabel);
     //freshlable();
    if (listwidgetcount<1){
        ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
        ui->statusBar->removeWidget(ui->treeView_3->statusLabel);
        ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
        this->setuplable();

        listwidgetcount++;
    }else{
        ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
        ui->statusBar->removeWidget(ui->treeView_3->statusLabel);
        ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
        this->freshlable();
    }
    //this->setuplable();
}

void ListWidget::showsortresult(){


    QStringList headlable;
    headlable<<ui->comboBox->currentText();
    this->addItems(my_sortlist);
    //my_sortlist.at(1);

    my_sortlist.clear();


}
