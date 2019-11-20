#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
class Button : public QPushButton
{
    Q_OBJECT
protected:
    void enterEvent(QEvent *);
signals:
    void mouseenter();
};
#endif // PUSHBUTTON_H
