#include "treeview.h"
#include "qdebug.h"
#include <QTimer>
#include "ui_mainwindow.h"
#include <myfilesytemmodel.h>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QClipboard>
#include <QMimeData>
#include <QFileDialog>
TreeView::TreeView(QWidget *parent) : QTreeView() //构造函数
{
    Q_UNUSED(parent);

    //this->setSelectionMode(QAbstractItemView::MultiSelection);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    //qDebug()<<"TreeView working";
    //connect(this, &QTreeView::doubleClicked, this, &TreeView::onTreeViewDoubleClicked);
    //connect(this,&QTreeView::doubleClicked, this, &TreeView::onTreeViewDoubleClicked);
    //connect(this, &QTreeView::clicked, this, &TreeView::on_treeView_clicked);

    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(slotCustomContextMenu(const QPoint &)));
    //connect(&timer,SIGNAL(timeout()),this,SLOT(onSingleClick()));
    QString init;
//    init = "***";
//    setuplable(init);


}

void TreeView::slotCustomContextMenu(const QPoint &point) //槽函数定义
{
    QMenu *menu = new QMenu(this);
    QAction *open=new QAction(QIcon(":/resource/openFile.png"),tr("Open"));
    menu->addAction(open);


//    QModelIndex index = this->currentIndex();
//    QFileSystemModel *model=new QFileSystemModel(this);
//    QString fileName=model->filePath(index);
//    QFileInfo info(fileName);
//    if(info.isDir())
//    {
//        open->setEnabled(false);
//    }


//    open->setEnabled(false);

    QAction *deleteFile=new QAction(QIcon(":/resource/delete.png"),tr("Delete"));
    menu->addAction(deleteFile);

    //分割线
    QAction *sp=new QAction(this);
    sp->setSeparator(true);
    menu->addAction(sp);

    QAction *copyPath=new QAction(QIcon(":/resource/copyPath.png"),tr("Copy path"));
    menu->addAction(copyPath);
    QAction *rename=new QAction(QIcon(":/resource/rename.png"),tr("Rename"));
    menu->addAction(rename);    
    QAction *copy=new QAction(QIcon(":/resource/copy.png"),tr("Copy to"));
    menu->addAction(copy);
    QAction *cut=new QAction(QIcon(":/resource/cut.png"),tr("Cut"));
    menu->addAction(cut);
//    QAction *paste=new QAction(tr("Paste"));
//    menu->addAction(paste);
    QAction *copytoclip=new QAction(QIcon(":/resource/copyToClipboard.png"),tr("Copy to clipboard"));
    menu->addAction(copytoclip);
    QAction *newFolder=new QAction(QIcon(":/resource/newFolder.png"),tr("New folder"));
    menu->addAction(newFolder);



    //右键open
    connect(open, SIGNAL(triggered()), this, SLOT(openFile()));

    //右键delete
    connect(deleteFile, SIGNAL(triggered()), this, SLOT(deleteFile()));

    //右键rename
    connect(rename, SIGNAL(triggered()), this, SLOT(renameFile()));

    //右键copy path
    connect(copyPath, SIGNAL(triggered()), this, SLOT(copy()));

    //copy to
    connect(copy , SIGNAL(triggered()), this, SLOT(copyFile()));

    //copy to clipboard
    connect(copytoclip,SIGNAL(triggered()),this,SLOT(copyToClipboard()));

    //new folder
    connect(newFolder,SIGNAL(triggered()),this,SLOT(newFolder()));

    //cut
    connect(cut,SIGNAL(triggered()),this,SLOT(cut()));
    menu->exec(this->mapToGlobal(point));


}

//打开文件的槽函数
void TreeView::openFile()
{

    qDebug()<<"执行openFile";
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    open(fileName);
}

void TreeView::copyToClipboard()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    QList<QUrl> copyfile;
    /*QDir sbPath = model->fileInfo(index).absoluteDir();
    QString sbbpath = sbPath.absolutePath();*/
    QUrl url=QUrl::fromLocalFile(fileName);
    if(url.isValid()){
        copyfile.push_back(url);
    }
    else
        return;
    QMimeData *data=new QMimeData;
    data->setUrls(copyfile);

    QClipboard *clip=QApplication::clipboard();
    clip->setMimeData(data);


}

void TreeView::open(QString fileName)
{
    QFileInfo info(fileName);
//    QFileSystemModel *model=new QFileSystemModel(this);
    if(info.isDir())
    {
        QModelIndex index = this->currentIndex();
        expand(index);

//        QDir *dir=new QDir();
//        model->setFilter(Dir::n);
        qDebug()<<"被点了一下";
        qDebug()<<info.isDir();
//        ui->treeView_3->clearMask();
//        ui->treeView_3->setRootIndex(model->setRootPath(fileName));
//        dir->cd(info.fileName());
//        qDebug()<<info.fileName();
//        div->cd("Cannot delete!!!");

//        QDesktopServices::openUrl(QUrl("file:///" + fileName));

    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}


//删除文件的槽函数
void TreeView::deleteFile()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    warningMessageBox(index,model);
    QDir filedir = model->fileInfo(index).absolutePath();
    QString filepath = filedir.absolutePath();
    //setuplable(filepath);
    freshlable(filepath);
}

//删文件时弹出警告
void TreeView::warningMessageBox(QModelIndex &index,QFileSystemModel *model)
{
    if(QMessageBox::Yes==QMessageBox::warning(this,"Delete file","Are you sure to delete this file?",QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel))
    {
        qDebug()<<"点击yes";
        model->remove(index);
    }
    else
    {
        return;
    }
}

//重命名
void TreeView::renameFile()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    QString name=model->fileName(index);
    QString oldName=fileName;
    QString partFileName=model->fileName(index);
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
                QDir sbPath = model->fileInfo(index).absoluteDir();
                QString sbbpath = sbPath.absolutePath();

                QDir dir;

//                fileName.replace(partFileName,newName);

                QFileInfo info(oldName);
                QString fileSuffix=info.suffix();
                QString finallNewName=sbbpath+"/"+newName+"."+fileSuffix;
                qDebug()<<fileSuffix;
                qDebug()<<finallNewName;
                dir.rename(oldName,finallNewName);
            }
            else
            {
                QMessageBox::critical(this,"Rename file","Invalid file name",QMessageBox::Yes);
            }
        }
    }

//    if(newName.isEmpty())
//    {
//        QMessageBox::critical(this,"Rename file","Invalid file name",QMessageBox::Yes);
//    }
//    else
//    {
//        if(ok&&isCorrectNewName(newName))
//        {
//            QDir dir;
//            fileName.replace(partFileName,newName);
//            QFileInfo info(oldName);
//            QString fileSuffix=info.suffix();
//            QString finallNewName=fileName+"."+fileSuffix;
//            qDebug()<<fileSuffix;
//            qDebug()<<finallNewName;
//            dir.rename(oldName,finallNewName);
//        }
//        if(!isCorrectNewName(newName))
//        {
//            QMessageBox::critical(this,"Rename file","Invalid file name",QMessageBox::Yes);
//        }
//    }

}
bool TreeView::isCorrectNewName(QString newName)
{
    if(newName.contains("/")||newName.contains("\\")||newName.contains(":")||newName.contains("*")||newName.contains("?")||newName.contains("<")||newName.contains(">")||newName.contains("|"))
    {
        return false;
    }
    return true;
}

//复制路径
void TreeView::copy()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(fileName);
}

void TreeView::onTreeViewDoubleClicked(const QModelIndex &index, QFileSystemModel *model)
{
    QString target;
    //filemodel2=new QFileSystemModel();
    QString path=ui->lineEdit->text();
    model->setRootPath(path);



    this->setModel(model);
    auto filemodel = new QFileSystemModel(this);
    QFileInfo info = filemodel->fileInfo(index);
    qDebug() << QString("open %1").arg(info.filePath());
    target = info.absoluteFilePath();
    QFileInfo file(target);
    if(file.isFile()){
        QDesktopServices::openUrl(QUrl::fromLocalFile(target));}
    else {
        if(file.isDir()){
            this->setRootIndex(model->index(path));
            //ui->tabWidget->setCurrentIndex(1);
            ui->lineEdit_3->setText(path);
        }
    }
    //openFile(target);

}
int i =0;
QString TreeView::on_treeView_clicked(const QModelIndex &index)
{
    ui->lineEdit->clear();
//    ui->statusBar->removeWidget(statusLabel);
//    ui->statusBar->addWidget(statusLabel);

    auto filemodel = new QFileSystemModel(this);
    QString sPath = filemodel->fileInfo(index).absoluteFilePath();

    this->setRootIndex(filemodel->setRootPath(sPath));
    ui->lineEdit->setText(sPath);

    qDebug()<<this->currentIndex().row();
    //    qDebug()<<ui->treeView_3->currentIndex().column();
    //    qDebug()<<ui->treeView_3->currentIndex().model();
    QModelIndex selected = this->currentIndex(); //选中的行
    selected = selected.sibling(selected.row(),0); //0 就是第一列元素，1就是第二列元素，依此类推
    QString strColumn0(this->model()->itemData(selected).values()[0].toString()); //由你自己每一列的QVariant绑定的值，决定获取数据的方式
    qDebug()<<strColumn0;
    QDir pppath = filemodel->fileInfo(index).absolutePath();
    QString path = pppath.absolutePath();


    //qDebug()<<sPath+newName;
    //QString curPath = dir.absolutePath();
    //QString strpath(pppath)
    QString ppath = path+"/"+strColumn0;
    return ppath;

//    if(ui->treeviewclicledcount<1){
        //setuplable(ppath);
        //ui->treeviewclicledcount++;
//    }
//    else{
//        freshlable(ppath);
//    }


    qDebug()<<"============="<<path+"/"+strColumn0;

}
void  TreeView::setuplable(QString path){

    statusLabel=new QLabel(this);
    //int i = this->count();






    //qDebug()<<i;
    //int i =10;
    QFileInfo fileinfo(path);
    if(fileinfo.isDir()){
        QDir file(path);
        QStringList filelist = file.entryList();
        int i = filelist.size()-2;
//        QString text = QString::number(i);
        QString text=tr(" %n item(s)", "number of the item", i);
         statusLabel->setText(text);
    }
//    else{
//        if(path=="***"){
//            QString text = path;
//            statusLabel->setText(text);
//        }
//    }
    //QString tt = "---";

    //ui->listWidget_4->statusLabel->se
    //QWidget *statuwidget;
    //ui->statusBar->close();
    //ui->statusBar->windowState();
    ui->statusBar->removeWidget(statusLabel);
    ui->statusBar->addWidget(statusLabel);

}
void TreeView::freshlable(QString filepath){
    ui->statusBar->removeWidget(statusLabel);
    ui->statusBar->addWidget(statusLabel);
    setuplable(filepath);
}

bool TreeView::copyFile()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString oldName=model->filePath(index);
    //QString currentfilepath = model->fileInfo(index).absoluteFilePath();
    qDebug()<<"oldname"<<oldName;
    QString dstFolderPath=QFileDialog::getExistingDirectory(this,"Select a folder to paste",oldName);
    qDebug()<<"这个是选中的目录"<<dstFolderPath;


    QFileInfo *file=new QFileInfo(oldName);
    QString fileName=file->baseName()+"."+file->suffix();
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



    return 0;
}


void TreeView::cut()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    QString dstFolderPath=QFileDialog::getExistingDirectory(this,"Select a folder to paste");
    qDebug()<<dstFolderPath;
    QDir sbPath = model->fileInfo(index).absoluteDir();
    QString sbbpath = sbPath.absolutePath();
    //dstFolderPath.replace("\\","/");
    //        if (sbbpath == dstFolderPath){
    //            return true;
    //        }
    //        if (!QFile::exists(sbbpath)){
    //            return false;
    //        }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(dstFolderPath);
    if (exist){
        createfile->remove(dstFolderPath);
    }
    QFileInfo info(fileName);
    dstFolderPath=dstFolderPath+'/'+info.fileName();
    QFile currentFile(fileName);
    bool ok=currentFile.copy(dstFolderPath);
    qDebug()<<fileName;
    if(ok)
        qDebug()<<"finished copy";
    else
        qDebug()<<"can't copy";
    //qDebug()<<info.fileName();


    if(ok)
    {
        //warningMessageBox(index,model);
        model->remove(index);
        QDir filedir = model->fileInfo(index).absolutePath();
        QString filepath = filedir.absolutePath();
        //setuplable(filepath);
        freshlable(filepath);
    }

}

QString TreeView::ifSameName(QString _SnapPath,QDir *snapFile,char i)
{
    if(i!='1')
    {
        _SnapPath=_SnapPath.left(_SnapPath.size());
        _SnapPath+=" -copy";
    }
    bool exist = snapFile->exists(_SnapPath);
    if (exist)
    {
        qDebug() << "exist" << _SnapPath;
        _SnapPath+=i;
        qDebug()<<_SnapPath;
        return ifSameName(_SnapPath,snapFile,i);
    }
    else
        return _SnapPath;

}

void TreeView::newFolder()
{
    QModelIndex index = this->currentIndex();
    QFileSystemModel *model=new QFileSystemModel(this);
    QString fileName=model->filePath(index);
    /*QString folderName="New Folder";
    //QString  dir_str = "C:\\test2\\test";
    QDir sbPath = model->fileInfo(index).absoluteDir();
    //QDir dir(QDir::currentPath());
           if (!sbPath.exists(folderName))
           {
                 bool res = sbPath.mkdir(folderName);
                 qDebug()<<folderName;
           }
           else
               qDebug()<<"no";*/
    //QString m_srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory","/");
    QString m_srcDirPath = QFileDialog::getExistingDirectory(this, "choose src Directory",fileName);
    if (m_srcDirPath.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() << "srcDirPath=" << m_srcDirPath;

        m_srcDirPath += "/";
        std::string m_path = m_srcDirPath.toStdString();
        std::string temp_Path = m_path + "New Folder";
        QString _SnapPath = QString::fromStdString(temp_Path);
        QDir *snapFile = new QDir;
        _SnapPath=ifSameName(_SnapPath,snapFile,'1');
        bool ok=snapFile->mkdir(_SnapPath);
        if(ok)
            qDebug()<<"ok";
        else
            qDebug()<<"no";
        /*bool exist = snapFile->exists(_SnapPath);
           if (exist)
           {
               qDebug() << "exist" << _SnapPath;
               bool ok=snapFile->mkdir(_SnapPath+="(1)");
           }
           else
           {
               bool ok = snapFile->mkdir(_SnapPath);
               if (ok)
               {
                   qDebug() << "ok";
               }
               else
               {
                   qDebug() << "no";
               }
           }*/
    }

}

/*    int treeviewclicledcount3=0;
    int treeviewclicledcount=0;
    int treeviewclicledcount2=0;
    int tablewidgetcount=0;
    int listwidgetcount=0;*/
