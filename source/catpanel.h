#ifndef RPANEL_H
#define RPANEL_H
#include <QWidget>
#include "button.h"
#include "cchar.h"
#include "charset.h"
class MainWindow;
class CatPanel:public QWidget
{
    Q_OBJECT
public:
    CatPanel(MainWindow *m,QFont &font);
    void load(const Charset* cs);
private:
    Button *button[12][12];
    CChar Char[12][12];
    void SetVK(int x,int y,QString tag,unsigned char VK);
    void SetChar(int x,int y,QString tag,wchar_t wc);
    void SetCatalog(int x,int y,QString tag,size_t cid);
    void SetIntenal(int x,int y,QString tag,size_t cmd);
    MainWindow *main;
    //Do not use signals, as class CChar is not a sub-class of class QObject.
};

#endif // RPANEL_H
