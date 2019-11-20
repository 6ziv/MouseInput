#ifndef BUTTOMBAR_H
#define BUTTOMBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "button.h"
class MainWindow;
class BottomBar:
        public QWidget
{
    Q_OBJECT
public:
    BottomBar(MainWindow *parent);
    void loadstrings(const std::vector<std::wstring>& strlist);
    void loadlabel(const QString text);
private:
    std::wstring str[10];
    Button *button[10];
    QLabel *label;
    MainWindow *m;
signals:
    void clear();
};

#endif // BUTTOMBAR_H
