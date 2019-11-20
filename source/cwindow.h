#ifndef CWINDOW_H
#define CWINDOW_H

#include <QWidget>

class CWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CWindow(QWidget *parent = nullptr);
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private:
    bool IsDragging;
    QPoint pt;
signals:
    void mouseleave();

public slots:
};

#endif // CWINDOW_H
