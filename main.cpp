#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QtGui>
#include <QDockWidget>
#include <QWidget>
#include <boost/thread.hpp>
using namespace boost;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setVisible(1);
    return a.exec();
}
