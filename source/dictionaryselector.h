#ifndef DICTIONARYSELECTOR_H
#define DICTIONARYSELECTOR_H

#include <QDialog>
#include <QApplication>
#include <QCheckBox>
#include <QPushButton>
#include <QSettings>
class DictionarySelector : public QDialog
{
public:
    DictionarySelector(QApplication *app,QSettings *settings);
private:
    QCheckBox* cb[11];
    QPushButton *build;
    QPushButton *select;
    QApplication *application;
};

#endif // DICTIONARYSELECTOR_H
