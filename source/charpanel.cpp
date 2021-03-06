#include "charpanel.h"
#include "mainwindow.h"
#include <qdebug.h>
CharPanel::CharPanel(MainWindow *m,QFont &font)
    :QWidget(m)
{
    main=m;
    this->resize(240,240);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            button[i][j]=new Button();
            button[i][j]->setParent(this);
            button[i][j]->setGeometry(i*30,j*30,30,30);
            button[i][j]->setFont(font);
            button[i][j]->show();
            connect(button[i][j],&Button::pressed,this,[=](){
                main->charpressed(Char[i][j]);
            });
        }
    }
}
void CharPanel::SetChar(int x,int y,const QString tag,wchar_t wc){
    QString tag1=tag;
    button[x][y]->setText(tag1.replace('&',"&&"));
    Char[x][y].type=CChar::CChar_Char;
    Char[x][y].ch.ch=wc;
}
void CharPanel::SetIntenal(int x,int y,const QString tag,size_t cmd){
    QString tag1=tag;
    button[x][y]->setText(tag1.replace('&',"&&"));
    Char[x][y].type=CChar::CChar_Internal;
    Char[x][y].ch.cmd=cmd;
}
void CharPanel::load(const std::vector<wchar_t>& table,size_t page){
    clear();
    const std::wstring tags[4]={L"首页",L"上页",L"下页",L"末页"};
    size_t pages=(table.size()-1)/60+1;
    size_t go_page[4]={0,(page>0)?page-1:0,page<(pages-1)?page+1:pages-1,pages};
    if(table.size()<=64){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                size_t id=static_cast<size_t>(8*i+j);
                if(id<table.size())
                    SetChar(j,i,QString()+QChar(table.at(id)),table.at(id));
            }
        }
    }else{
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(i==7 && j<6 && j>1){
                    SetIntenal(j,i,QString::fromStdWString(tags[j-2]),go_page[j-2]);
                    continue;
                }
                size_t id=static_cast<size_t>(8*i+j)+page*60;
                if(i==7 && j>5)
                    id-=4;
                if(id<table.size())
                    SetChar(j,i,QString()+QChar(table.at(id)),table.at(id));
            }
        }
    }
}
void CharPanel::clear(){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Char[i][j].type=CChar::CChar_Invalid;
            button[i][j]->setText("");
        }
    }
}
QPoint CharPanel::center(){
    return button[3][3]->geometry().center();
}
