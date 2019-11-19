#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include "cchar.h"
#include "charset.h"
#include "pushbutton.h"
#include "dictionary.h"
#include <memory>
#include <QLabel>
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent,QApplication *app);
    ~MainWindow();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void catpreview(int x,int y);
private:
    void ImaginePressed(int id);
    std::wstring im[10];
    void catpressed(int x,int y);
    bool IsDragging;
    QPoint pt;
    CChar CatChar[12][12];
    CChar CharPad[8][8];
    void SetVK(int x,int y,QString tag,unsigned char VK);
    void SetChar(int x,int y,QString tag,wchar_t wc);
    void SetCatalog(int x,int y,QString tag,size_t cid);
    void SetIntenal(int x,int y,QString tag,unsigned char cmd);

    void GoToBtn(QPushButton *pb);
    std::vector<std::unique_ptr<charset>> charsets;
    std::vector<std::unique_ptr<charset>>::iterator current_charset;

    std::unique_ptr<Dictionary> dictionary;
    void ShowCat(int cid,int page=0);
    void ShowImagine(wchar_t ch);
    int current_cid;
    friend class PushButton;

    QPushButton *btnChar[8][8];
    PushButton *btnCatalog[12][12];
    QPushButton *btnImagine[10];
    QLabel *LongTab;
    std::unique_ptr<QSettings> settings;
    void LoadCats();
    QApplication * application;
};
#endif // MAINWINDOW_H
