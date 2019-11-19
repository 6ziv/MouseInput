#include "pushbutton.h"
#include <Windows.h>
#include <mainwindow.h>
PushButton::PushButton(MainWindow * parent,int x,int y)
    :QPushButton(parent){
    X=x;
    Y=y;
    p=parent;
}
void PushButton::enterEvent(QEvent *){
     p->catpreview(X,Y);
}
