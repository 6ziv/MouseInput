#ifndef CHARPANEL_H
#define CHARPANEL_H
#include <QPushButton>
#include "cchar.h"
#include "button.h"
class MainWindow;
class CharPanel:public QWidget
{
    Q_OBJECT
public:
    CharPanel(MainWindow *,QFont &);
    void load(const std::vector<wchar_t>& table,size_t page);
    void clear();
    QPoint center();
private:
    Button *button[8][8];
    CChar Char[8][8];
    void SetChar(int x,int y,const QString tag,wchar_t wc);
    void SetIntenal(int x,int y,const QString tag,size_t cmd);
    MainWindow *main;

};

#endif // CHARPANEL_H
