#include "cwindow.h"
#include <QMouseEvent>
#include <Windows.h>
CWindow::CWindow(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::WindowDoesNotAcceptFocus);
    this->setWindowOpacity(0.5);
    this->resize(675,280);
    SetWindowLong(HWND(this->winId()),GWL_EXSTYLE,GetWindowLong(HWND(this->winId()),GWL_EXSTYLE)|WS_EX_TOPMOST | WS_EX_NOACTIVATE);
}
void CWindow::enterEvent(QEvent *){
    this->setWindowOpacity(1);
}
void CWindow::leaveEvent(QEvent *){
    this->setWindowOpacity(0.4);
    emit mouseleave();
}
bool CWindow::nativeEvent(const QByteArray &, void *message, long *r){
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
        MSG *e = *reinterpret_cast<MSG**>(message);
#else
        MSG *e = reinterpret_cast<MSG*>(message);
#endif
        if(e->message==WM_MOUSEACTIVATE){
            *r=MA_NOACTIVATE;
            return true;
        }
        return false;
}
void CWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()!=Qt::LeftButton)return;
    IsDragging=true;
    pt=e->pos();
}
void CWindow::mouseReleaseEvent(QMouseEvent *){
    IsDragging=false;
}
void CWindow::mouseMoveEvent(QMouseEvent *e){
    if(!e->buttons().testFlag(Qt::MouseButton::LeftButton)){
        IsDragging=false;
        return;
    }
    if(!IsDragging)
        return;
    this->move(this->pos()+e->pos()-pt);
}

