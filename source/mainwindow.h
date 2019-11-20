#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include "cwindow.h"
#include "cchar.h"
#include "charset.h"
#include "button.h"
#include "dictionary.h"
#include "charsetselector.h"
#include "catpanel.h"
#include "charpanel.h"
#include "bottombar.h"
#include <memory>
#include <QLabel>
class MainWindow : public CWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent,QApplication *app);
    ~MainWindow();
    void catpreview(const CChar &c);
    void catpressed(const CChar &c);
    void charpressed(const CChar &c);
    void clear();
protected:
    void mouseReleaseEvent(QMouseEvent *e);

private:
    std::unique_ptr<CharsetSelector> cssel;
    CatPanel *cp;
    CharPanel *chp;
    BottomBar *bt;
    size_t current_cid;
    bool imagined;

    std::unique_ptr<Dictionary> dictionary;
    void ShowCat(size_t cid,size_t page=0);
    void ShowImagine(wchar_t ch);
    std::unique_ptr<QSettings> settings;
    QApplication * application;
};
#endif // MAINWINDOW_H
