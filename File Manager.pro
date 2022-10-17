QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filewidget.cpp \
    findwidget.cpp \
    listwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    myfilesytemmodel.cpp \
    mytabwidget.cpp \
    tab_page.cpp \
    tablewidget.cpp \
    treeview.cpp

HEADERS += \
    filewidget.h \
    findwidget.h \
    listwidget.h \
    mainwindow.h \
    myfilesytemmodel.h \
    mytabwidget.h \
    tab_page.h \
    tablewidget.h \
    treeview.h

FORMS += \
    mainwindow.ui

RC_ICONS +=filemanager.ico
\
#    resources/icon_app.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
