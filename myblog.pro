#-------------------------------------------------
#
# Project created by QtCreator 2014-03-03T18:14:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myblog
TEMPLATE = app

INCLUDEPATH += /usr/local/Cellar/boost/1.55.0/include

LIBS += /usr/local/Cellar/boost/1.55.0/lib/libboost_system.a\
        /usr/local/Cellar/boost/1.55.0/lib/libboost_filesystem.a\
        /usr/local/Cellar/boost/1.55.0/lib/libboost_date_time.a\

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    myicons.icns

ICON = myicons.icns

