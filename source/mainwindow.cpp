#include "mainwindow.h"
#include <QMouseEvent>
#include <Windows.h>
#include "sendchar.h"
#include <QSettings>
#include <QFileInfo>
#include <QScreen>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent,QApplication *app)
    : CWindow(parent)
{
    application=app;
    settings=std::make_unique<QSettings>(QApplication::applicationDirPath()+"/settings.ini",QSettings::IniFormat);
    dictionary=std::make_unique<Dictionary>();
    if(settings->contains("dictionary")){
        dictionary->open(settings->value("dictionary","").toString().toStdWString());
    }
    std::vector<std::wstring> filenames;
    filenames.clear();
    if(settings->contains("charset")){
        auto cslist=settings->value("charset","").toStringList();
        for(auto cs : cslist){
            filenames.push_back(cs.toStdWString());
        }
    }
    cssel=std::make_unique<CharsetSelector>(filenames);

    if(settings->contains("fav_charset")){
        bool ok;
        auto id=settings->value("fav_charset",0).toUInt(&ok);
        if(ok)
            cssel->select(id);
        else
            cssel->select(0);
    }


    auto favpos=settings->value("fav_position",QPoint(QApplication::screens().at(0)->size().width()-675,QApplication::screens().at(0)->size().height()-300)).toPoint();
    this->move(favpos);

    QFont font1;
    QFont font2=font1;
    font1.setPixelSize(10);

    cp=new CatPanel(this,font1);
    cp->move(250,5);
    cp->show();

    chp=new CharPanel(this,font2);
    chp->move(5,5);
    chp->show();

    bt=new BottomBar(this);
    bt->move(5,250);
    bt->show();
    connect(bt,&BottomBar::clear,this,[=](){clear();});
    connect(this,&CWindow::mouseleave,[=](){clear();});
    cp->load(cssel->get());
    clear();
    this->show();
}

MainWindow::~MainWindow()
{
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    CWindow::mouseReleaseEvent(e);
    settings->sync();
    settings->setValue("fav_position",this->pos());
    settings->sync();
}

void MainWindow::catpreview(const CChar& ch){
    if(ch.type!=CChar::CChar_Catalog){
        clear();
    }else{
        ShowCat(ch.ch.cid);
    }
}

void MainWindow::catpressed(const CChar& ch){
    catpreview(ch);

    if(ch.type==CChar::CChar_VK){
        SendChar::SendVK(ch.ch.vk);
    }
    if(ch.type==CChar::CChar_Char){
        SendChar::SendChar(ch.ch.ch);
    }
    if(ch.type==CChar::CChar_Catalog){
        QPoint target=chp->center()+chp->pos()+this->pos();
        SetCursorPos(target.x(),target.y());
    }
    if(ch.type==CChar::CChar_Internal){
        if(ch.ch.cmd==255)ExitProcess(0);
        if(ch.ch.cmd==254){
            cssel->move();
            settings->setValue("fav_charset",cssel->pos());
            settings->sync();
            cp->load(cssel->get());
            clear();
        }
        if(ch.ch.cmd==253){
            QString path=QFileDialog::getOpenFileName(this,"Select charset",QApplication::applicationDirPath(),"Charset (*.cst);;Text files (*.txt);;All files (*.*)");
            if(path.isEmpty())return;
            settings->sync();
            QStringList sl=settings->value("charset","").toStringList();
            if(sl.contains(path)){
                QMessageBox(QMessageBox::Icon::Warning,"Error","Duplicate charsets",QMessageBox::Ok,this).exec();
                return;
            }
            if(cssel->open(path.toStdWString())){
                sl.push_back(path);
                settings->setValue("charset",sl);
                settings->sync();
            }else{
                QMessageBox(QMessageBox::Icon::Warning,"Error","Can not open charset",QMessageBox::Ok,this).exec();
            }
        }
        if(ch.ch.cmd==252){
            QString path=QFileDialog::getOpenFileName(this,"Select charset",QApplication::applicationDirPath(),"Charset (*.cst);;Text files (*.txt);;All files (*.*)");
            if(path.isEmpty())return;
            auto dict=std::make_unique<Dictionary>();
            if(dict->open(path.toStdWString())){
                settings->sync();
                settings->setValue("dictionary",path);
                settings->sync();
                dictionary=std::move(dict);
            }
            else{
                QMessageBox(QMessageBox::Icon::Warning,"Error","Can not open dictionary",QMessageBox::Ok,this).exec();
            }
        }
        if(ch.ch.cmd==251){
            size_t pos=cssel->pos();
            if(!cssel->close()){
                QMessageBox(QMessageBox::Icon::Warning,"Error",QString::fromStdWString(L"不能移除默认字库"),QMessageBox::Ok,this).exec();
                return;
            }
            settings->sync();
            QStringList sl=settings->value("charset","").toStringList();
            sl.removeAt(static_cast<int>(pos));
            settings->setValue("charset",sl);
            settings->sync();
            cp->load(cssel->get());
            clear();
        }
    }
}

void MainWindow::ShowCat(size_t cid,size_t page){
    imagined=false;
    current_cid=cid;
    bt->loadlabel(QString::fromStdWString(
                      cssel->get()->GetCatalogDescription(cid))
                  );
    chp->load(cssel->get()->GetCatalog(cid),page);
}
void MainWindow::clear(){
    bt->loadlabel("");
    chp->clear();
}
void MainWindow::ShowImagine(wchar_t ch){
    imagined=true;
    for(size_t i=0;i<8;i++){
        for(size_t j=0;j<8;j++){
            chp->load(dictionary->get2(ch),0);
        }
    }
    bt->loadstrings(dictionary->get(ch));
}
void MainWindow::charpressed(const CChar &ch){
    if(ch.type==CChar::CChar_Char){
        SendChar::SendChar(ch.ch.ch);
        if(imagined)
            clear();
        else
            ShowImagine(ch.ch.ch);
        return;
    }
    if(ch.type==CChar::CChar_Internal){
        ShowCat(current_cid,ch.ch.cmd);
        return;
    }
}
