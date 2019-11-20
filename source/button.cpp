#include "button.h"
#include <Windows.h>
#include <mainwindow.h>
void Button::enterEvent(QEvent *){
    emit this->mouseenter();
}
