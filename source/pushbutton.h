#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
class MainWindow;
class PushButton : public QPushButton
{
    Q_OBJECT
public:
    PushButton(MainWindow *,int,int);
protected:
    void enterEvent(QEvent *);
private:
    int X;int Y;
    MainWindow *p;
};

#endif // PUSHBUTTON_H
