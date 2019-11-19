#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <Windows.h>
#include "dictionaryselector.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    if(QFile::exists(QApplication::applicationDirPath()+"/firstrun")){
        QFile::remove(QApplication::applicationDirPath()+"/firstrun");
        QSettings settings(QApplication::applicationDirPath()+"/settings.ini",QSettings::IniFormat);
        DictionarySelector ds(&a,&settings);
        ds.exec();
    }
    MainWindow w(nullptr,&a);
    w.show();
    return a.exec();
}
