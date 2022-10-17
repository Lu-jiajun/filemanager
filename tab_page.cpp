#include "tab_page.h"
#include "treeview.h"

#include <QTreeWidget>
Tab_Page::Tab_Page(QWidget *parent)
{
    Q_UNUSED(parent);
    setupTab_page();

}

void Tab_Page::setupTab_page(){
    //QWindow baseWindow;
    QTreeView *newtreewidget = new QTreeView();
    //QTreeView newTreeView;
    //QWidget *base =new QWidget();
    QPushButton btn("newbtn",this);
    btn.show();
    this->show();

    //this->children(newtreewidget);
    //btn.setParent(&baseWindow);
    //btn.show();
}
