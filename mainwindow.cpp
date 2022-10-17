#include "mainwindow.h"
#include "findwidget.h"
#include "qdebug.h"
#include "ui_mainwindow.h"
#include "tab_page.h"
#include <QAction>
#include <QMessageBox>
#include <filewidget.h>
#include <QTextStream>
#include <QFile>
#include <QDataStream>
#include <QBitArray>
#include <QDirModel>
#include <myfilesytemmodel.h>
#include <QDesktopServices>
#include <QUrl>
#include <QTreeWidgetItem>
#include <QList>
#include <tablewidget.h>
#include <QString>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMimeDatabase>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QLayout>

QStringList my_sortlist;

enum { absoluteFileNameRole = Qt::UserRole + 1 };

static inline QString fileNameOfItem(const QTableWidgetItem *item)
{
    return item->data(absoluteFileNameRole).toString();
}

MainWindow* MainWindow::mywidget = nullptr;//在其他类中调用ui指针
//MainWindow* MainWindow::mytreeview = nullptr;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    Tab_Page test;
//    test.show();

    //获取主屏分辨率 窗口为屏幕长宽的一半
    QRect mRect;
    mRect = QGuiApplication::primaryScreen()->geometry();
    qDebug()<<"width:"<<mRect.width()<<"  height:"<<mRect.height();
    setMinimumSize(mRect.width()/2,mRect.height()/2);
    //statusBar->addWidget(ui->treeView_2->statusLabel);

    //设置按钮字体
    setStyleSheet("QPushButton{""font-family:Microsoft Yahei;""}");



    mywidget = this;
    ui->setupUi(this);
    setupMenuBar();
    QString rootpath = "/";
    dirmodel = new MyFileSytemModel(this);
    dirmodel->setRootPath(rootpath);

    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel_4 = new QFileSystemModel(this);
    filemodel = new QFileSystemModel(this);
    filemodel2=new QFileSystemModel(this);
    filemodel->setRootPath(rootpath);

    //filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    ui->dockWidget->setWindowTitle(QString("Navigator"));
    //ui->dockWidget_3->setWindowTitle(QString("filewidget"));
    ui->treeView_2->setModel(dirmodel);

    ui->treeView_3->setModel(filemodel);



    ui->treeView->setModel(filemodel);
    ui->lineEdit->setText("/");
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"file"<<"size");
    qDebug()<<ui->treeView_2->currentIndex();
//    ui->treeView_2->setCurrentIndex(dirmodel->setRootPath("\\"));
//    ui->treeView_3->setCurrentIndex(filemodel->setRootPath("\\"));


    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::go);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::back);

    connect(ui->pushButton_3,&QPushButton::clicked,ui->listWidget_4,&ListWidget::sortbytype);
//    connect(ui->comboBox,&QComboBox::currentTextChanged,ui->listWidget_4,&ListWidget::sortbytype);
//    //connect(ui->pushButton_5,&QPushButton::clicked,this,&MainWindow::browser);
    connect(ui->pushButton_5,&QPushButton::clicked,ui->dockWidget_3,&FindWidget::browser);
    connect(ui->pushButton_4, &QAbstractButton::clicked, ui->dockWidget_3, &FindWidget::find);
    connect(ui->comboBox_3->lineEdit(), &QLineEdit::returnPressed,ui->dockWidget_3, &FindWidget::animateFindClick);

    connect(ui->treeView_3,&QTreeView::clicked,this,&MainWindow::on_treeView_3_clicked);
    connect(ui->treeView,&QTreeView::clicked,this,&MainWindow::on_treeView_clicked);


    connect(ui->lineEdit, &QLineEdit::returnPressed,ui->pushButton, &QPushButton::click);
    connect(ui->comboBox->lineEdit(),&QLineEdit::returnPressed,ui->pushButton_3, &QPushButton::click);
    //testTbaleaddItem();
    //tablewidgetinit();
    //testinsert();
    //connect(ui->treeView_3,&TreeView::open,this,&MainWindow::on_treeView_3_clicked);
    setupStatusBar();
    //connect(ui->treeView_3,&QTreeView::doublclicked,this,&MainWindow::on_treeView_3_clicked);
    connect(ui->treeView_3,&QTreeView::doubleClicked,this,&MainWindow::on_treeView_3_double_clicked);
    connect(ui->treeView,&QTreeView::doubleClicked,this,&MainWindow::on_treeView_double_clicked);

//    connect(ui->toolButton,&QToolButton::clicked,this,&MainWindow::onPrevClicked);
//    connect(ui->toolButton_2,&QToolButton::clicked,this,&MainWindow::onNextClicked);



//    //mytabaddtab();
//    connect(ui->toolButton_4,&QToolButton::clicked,this,&MainWindow::mytabaddtab);

//    connect(ui->toolButton_3,&QToolButton::clicked,this,&MainWindow::mytabremovetab);

    connect(ui->toolButton,&QToolButton::clicked,this,&MainWindow::cdParent);
    connect(ui->toolButton_2,&QToolButton::clicked,this,&MainWindow::backChild);

    setButton();
    ui->treeView_2->setuplable("/");
    treeviewclicledcount2++;
}

MainWindow::~MainWindow()
{
    delete ui;

}

//点击Jump to another view后从主视图转到副视图
void MainWindow::go()
{
    qDebug()<<"go";
    QString path=ui->lineEdit->text();
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

//    ui->treeView->setRootIndex(filemodel2->index(path));
//    ui->tabWidget->setCurrentIndex(1);
//    ui->lineEdit_3->setText(path);
//    qDebug()<<path;

}

//点击back返回主视图
void MainWindow::back()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::setupMenuBar(){

    QAction *exitAction = ui->menufile->addAction(tr("Exit"), qApp, &QCoreApplication::quit, Qt::QueuedConnection);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setShortcut(Qt::CTRL | Qt::Key_Q);



    ui->dockWidget->toggleViewAction()->setText(tr("Navigation Bar"));
    ui->dockWidget->toggleViewAction()->setShortcut(Qt::CTRL | Qt::Key_N);
    ui->menuview->addAction(ui->dockWidget->toggleViewAction());
    ui->dockWidget_3->toggleViewAction()->setText(tr("Find Dock"));
    ui->dockWidget_3->toggleViewAction()->setShortcut(Qt::CTRL | Qt::Key_F);
    ui->menuview->addAction(ui->dockWidget_3->toggleViewAction());

    //设置菜单栏图标
    exitAction->setIcon(QIcon(":/resource/exit.png"));
    ui->menuhelp->addAction(QIcon(":/resource/about.png"),tr("&About"), this, &MainWindow::about);

    //FileWidget *widget = (FileWidget *)centralWidget();
    MainWindow *widget= (MainWindow *)centralWidget();
    //QAction *addTabAct = ui->menuview->addAction(tr("Add &Tab"), widget, &MainWindow::addtab);
    //addTabAct->setShortcut(Qt::CTRL | Qt::Key_T);
    //connect(addtab, SIGNAL(triggered()), this, SLOT(slotaddtab(MainWindow*)))
    QStringList labels;
    labels << tr("Filename") << tr("Size");
    ui->tableWidget->setHorizontalHeaderLabels(labels);
//    MyTableWidget *mytablewidget;
//    mytablewidget->MyTableInit();

}
void MainWindow::about()
{
    static const char message[] =
        "<p><b>FileManagerDemo</b></p>"

        "<p>Version:&nbsp;Beta(x64)</p>"
        "<p>Author:&nbsp;&nbsp;Lu-jijun</p>"
        "<p>Date:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2022/10/01</p>"

        "<p></p>"
        "<p>Project:&nbsp;&nbsp;<a href=\"https://gitee.com/Lu_J1ajun/qtfilemanager\">Gitee repository</a>"
               ;

    //QMessageBox::about(this, tr("About"), message);
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About"));
    msgBox->setText(message);
    QPixmap pm(QLatin1String("./icon_app_64.ico"));
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->exec();
}

void MainWindow::on_treeView_2_clicked(const QModelIndex &index)
{
    ui->lineEdit->clear();
    QString ppath = ui->treeView_2->on_treeView_clicked(index);
    if(treeviewclicledcount2<1){
        ui->treeView_2->setuplable(ppath);
        treeviewclicledcount2++;
            }
            else{
                ui->treeView_2->freshlable(ppath);
            }
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    if(ui->tab->isActiveWindow()){
        ui->lineEdit->setText(sPath);
        ui->treeView_3->setRootIndex(filemodel->setRootPath(sPath));
        ui->treeView_3->setCurrentIndex(filemodel->setRootPath(sPath));

    }
     qDebug()<<ui->treeView_2->currentIndex().row();
//    if(ui->tab_3->isActiveWindow()){
        ui->lineEdit_3->setText(sPath);
//        ui->treeView->setRootIndex(filemodel->setRootPath(sPath));
//    }
//        if(ui->treeviewclicledcount>1){
//            ui->statusBar->removeWidget(ui->treeView_2->statusLabel);}
         ui->statusBar->removeWidget(ui->treeView_3->statusLabel);


}


void MainWindow::on_treeView_3_clicked(const QModelIndex &index)
{

//    if(ui->treeviewclicledcount3>1){
//        ui->statusBar->removeWidget(ui->treeView_3->statusLabel);}
    ui->treeView_3->on_treeView_clicked(index);
    ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
    //ui->treeviewclicledcount3++;
    ui->statusBar->removeWidget(ui->listWidget_4->statusLabel);
    ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
     ui->statusBar->removeWidget(ui->treeView->statusLabel);

    QString ppath = ui->treeView_3->on_treeView_clicked(index);
    qDebug()<<"~~~~~~~~~~~~~~~"<<ppath;
    bool isthepathdir;
    QFileInfo file(ppath);
    isthepathdir = file.isDir();
    if(treeviewclicledcount3<1){
        if(isthepathdir){
        ui->treeView_3->setuplable(ppath);
            treeviewclicledcount3++;}
        if(!isthepathdir){
            ui->treeView_3->setuplable(file.absoluteDir().absolutePath());
            treeviewclicledcount3++;
        }
    }
    else{
        if(treeviewclicledcount3>=1&&isthepathdir)
        ui->treeView_3->freshlable(ppath);
        if(treeviewclicledcount3>=1&&(!isthepathdir)){
            ui->treeView_3->freshlable(file.absoluteDir().absolutePath());
        }
    }
    updateButton();

}
void MainWindow::on_treeView_clicked(const QModelIndex &index)
{

    //    if(ui->treeviewclicledcount3>1){
    //        ui->statusBar->removeWidget(ui->treeView_3->statusLabel);}
    ui->treeView->on_treeView_clicked(index);
    ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
    //ui->treeviewclicledcount3++;
    ui->statusBar->removeWidget(ui->listWidget_4->statusLabel);
    ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
    ui->statusBar->removeWidget(ui->treeView_3->statusLabel);


    QString ppath = ui->treeView->on_treeView_clicked(index);
    qDebug()<<"~~~~~~~~~~~~~~~"<<ppath;
    bool isthepathdir;
    QFileInfo file(ppath);
    isthepathdir = file.isDir();
    if(treeviewclicledcount<1){
        if(isthepathdir){
            ui->treeView->setuplable(ppath);
            treeviewclicledcount++;}
        if(!isthepathdir){
            ui->treeView->setuplable(file.absoluteDir().absolutePath());
            treeviewclicledcount++;
        }
    }
    else{
        if(treeviewclicledcount>=1&&isthepathdir)
            ui->treeView->freshlable(ppath);
        if(treeviewclicledcount>=1&&(!isthepathdir)){
            ui->treeView->freshlable(file.absoluteDir().absolutePath());
        }
    }
}

void MainWindow::on_treeView_3_double_clicked(const QModelIndex &index){
    ui->treeView_3->onTreeViewDoubleClicked(index,filemodel);
//    QString path=filemodel->fileName(index);
//    historyPath.append(path);
//    pathCount++;
}
void MainWindow::on_treeView_double_clicked(const QModelIndex &index){
    ui->treeView->onTreeViewDoubleClicked(index,filemodel);
}

static  void openFile(const QString &fileName)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}





void MainWindow::showsortresult(){

    QStringList headlable;
    headlable<<ui->comboBox->currentText();
    ui->listWidget_4->addItems(my_sortlist);
    my_sortlist.clear();


}

void MainWindow::setupStatusBar(){
//    QString text;
//    text = "main window";
//    statusLabel->setText(text);

    //ui->statusBar->addWidget(statusLabel);
}

//设置状态栏可增减
//int tabcount = 2;
//void MainWindow::mytabaddtab(){
//    QFormLayout *formLayout;
//    QGroupBox *groupBox;
//    //ui->tabWidget->addTab(add,"3");
//    //auto QWindow *add = new QWindow;
//    //ui->tabWidget->addTab(add,"tab_4");
//    ui->tabWidget->addTab(new FileWidget,"test_tab");
//    tabcount++;


//    QWidget *tab=ui->tabWidget->widget(tabcount);
//    QString tabobjectname = QString::fromUtf8("tab")+ QString::number(tabcount);
//    tab->setObjectName(QString::fromUtf8("tab")+"_"+ QString::number(tabcount));
//    formLayout = new QFormLayout(tab);
//    formLayout->setObjectName(QString::fromUtf8("formLayout"));
//    groupBox = new QGroupBox(tab);
//    groupBox->setObjectName(QString::fromUtf8("groupBox"));

//    tab->toolTip().append("test");
//    //tab->setLayout()
//    //QPushButton testbutton("testbutton",tab);
//    //testbutton.show();

//    //qDebug()<<"is tab added "<<ui->tabWidget->insertTab(3,new QWidget,"test_tab");
//    //ui->tabWidget->removeTab(2);
//}
//void MainWindow::mytabremovetab(){

//    //ui->tabWidget->addTab(add,"3");
//    //auto QWindow *add = new QWindow;
//    //ui->tabWidget->addTab(add,"tab_4");
//    //ui->tabWidget->addTab(new Tab_Page,"test_tab");
//    //qDebug()<<"is tab added "<<ui->tabWidget->insertTab(3,new QWidget,"test_tab");
//    if(tabcount){
//    ui->tabWidget->removeTab(tabcount);
//        tabcount--;
//    }
//}

//设置跳转按钮
void MainWindow::setButton()
{
    QString text=ui->lineEdit->text();
    QFileInfo info(text);
//    QFileSystemModel *model=new QFileSystemModel(this);
    if(!info.isDir())
    {
        ui->pushButton->setText("Open");
    }
}

void MainWindow::updateButton()
{
    QString text=ui->lineEdit->text();
    QFileInfo info(text);
//    QFileSystemModel *model=new QFileSystemModel(this);
    if(!info.isDir())
    {
        ui->pushButton->setText("Open");
    }
    else
    {
        ui->pushButton->setText("Jump to secondary view");
    }
//    updateButton();
}

////返回上一级
//void MainWindow::onPrevClicked()
//{
//    int count=pathCount--;
//    QString path=historyPath.at(count);
//    QModelIndex index = filemodel->index(path);  // invalid index and index.parent()
//    ui->treeView_3->setRootIndex(index);
//}

//返回下一级
//void FileWidget::onNextClicked()
//{

//}

QStringList fileList;

//void MainWindow::cdParent(){

//    QModelIndex index = ui->treeView_3->currentIndex();
//    QString sPath = filemodel->fileInfo(index).absoluteFilePath();
//    if(sPath!="/")
//    {
//    QFileInfo child(sPath);
//    fileList.append(sPath);
//    QDir parent = child.absoluteDir();
//    QString parentpath = parent.absolutePath();
//    ui->treeView_3->setRootIndex(filemodel->setRootPath(parentpath));
//    ui->lineEdit->setText(parentpath);
//    //QFileInfo parentInfo(parentpath);
//    QModelIndex parentindex = ui->treeView_3->currentIndex();
//    qDebug()<<"index===="<<index;

//    qDebug()<<"parentindex===="<<parentindex;


//    ui->treeView_3->setCurrentIndex(filemodel->setRootPath(parentpath));
//    qDebug()<<"fileList::----"<<fileList;
//    }
//    else{
//        fileList.clear();
//        qDebug()<<fileList;
//    }

//}
//void MainWindow::backChild(){
//    //QModelIndex index = ui->treeView_3->currentIndex();
//    int count = fileList.size();
//    qDebug()<<count;
//    if(count>0){
//    QString sPath = fileList[count-1];
//    qDebug()<<"ooo"<<fileList[count-1];
//    fileList.removeAt(count-1);
//    ui->treeView_3->setRootIndex(filemodel->setRootPath(sPath));
//    }

//}


void MainWindow::cdParent(){

    QString sPath;
    QModelIndex index = ui->treeView_3->currentIndex();
    sPath = filemodel->fileInfo(index).absoluteFilePath();
    qDebug()<<"spath"<<sPath;
    if(sPath!="/")
    {


    QFileInfo child(sPath);
    fileList.append(sPath);
    QDir parent = child.absoluteDir();
    QString parentpath = parent.absolutePath();
    ui->treeView_3->setRootIndex(filemodel->setRootPath(parentpath));
    ui->treeView_3->setCurrentIndex(filemodel->setRootPath(parentpath));

    ui->lineEdit->setText(parentpath);
    //QFileInfo parentInfo(parentpath);
    QModelIndex parentindex = ui->treeView_3->currentIndex();
    qDebug()<<"index===="<<index;

    qDebug()<<"parentindex===="<<parentindex;
    qDebug()<<"fileList::----"<<fileList;

    if((treeviewclicledcount3!=0)){

    ui->statusBar->removeWidget(ui->listWidget_4->statusLabel);
    ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
    ui->statusBar->removeWidget(ui->treeView->statusLabel);
    ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
    QFileInfo file(sPath);
    ui->treeView_3->freshlable(file.absoluteDir().absolutePath());

    }
    if((treeviewclicledcount3==0)){
        ui->statusBar->removeWidget(ui->listWidget_4->statusLabel);
        ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
        ui->statusBar->removeWidget(ui->treeView->statusLabel);
        ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
        ui->treeView_3->setuplable("/");
        treeviewclicledcount3++;
    }
    }
    else{

        fileList.clear();
        fileList.append("/");
        qDebug()<<")()()("<<fileList;
    }
    updateButton();
//    ui->statusBar->removeWidget(ui->listWidget_4->statusLabel);
//    ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
//    ui->statusBar->removeWidget(ui->treeView->statusLabel);
//    QFileInfo file(sPath);
//    ui->treeView_3->freshlable(file.absoluteDir().absolutePath());

}
void MainWindow::backChild(){

    int count = fileList.size();
    qDebug()<<count;
    if(count>0){
    QString Path = fileList[count-1];
        ui->lineEdit->setText(Path);
    qDebug()<<"ooo"<<fileList[count-1];
    //QString ssPath = fileList[count];

    ui->treeView_3->setRootIndex(filemodel->setRootPath(Path));
    ui->treeView_3->setCurrentIndex(filemodel->setRootPath(Path));
    QModelIndex index = ui->treeView_3->currentIndex();
    ui->statusBar->removeWidget(ui->treeView_2->statusLabel);
    //ui->treeviewclicledcount3++;
    ui->statusBar->removeWidget(ui->listWidget_4->statusLabel);
    ui->statusBar->removeWidget(ui->tableWidget->statusLabel);
    ui->statusBar->removeWidget(ui->treeView->statusLabel);
    QString sPath = filemodel->fileInfo(index).absoluteFilePath();
    QFileInfo file(sPath);
//    ui->treeView_3->freshlable(file.absoluteDir().absolutePath());

    ui->treeView_3->freshlable(Path);

    fileList.removeAt(count-1);


    }
    updateButton();

}
