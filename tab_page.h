#ifndef TAB_PAGE_H
#define TAB_PAGE_H

#include "qtreeview.h"
#include <qwindow.h>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>


class Tab_Page : public QWidget
{
    Q_OBJECT
public:
    explicit Tab_Page(QWidget *parent = NULL);
    void setupTab_page();
    //Tab_Page();
    QTreeView *tab_treeview;
};

#endif // TAB_PAGE_H
