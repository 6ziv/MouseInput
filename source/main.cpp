#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <Windows.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    MainWindow w(nullptr,&a);
    w.show();
    return a.exec();
}
