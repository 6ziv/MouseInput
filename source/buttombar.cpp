#include "bottombar.h"
#include "mainwindow.h"
#include "sendchar.h"
#include <iostream>
BottomBar::BottomBar(MainWindow* parent)
    :QWidget(parent)
{
    this->resize(675,30);
    for(size_t i=0;i<10;i++){
        button[i]=new Button();
        button[i]->setParent(this);
        button[i]->hide();
        connect(button[i],&Button::pressed,this,[=](){
            SendChar::SendStr(str[i]);
            emit clear();
        });
    }
    this->label=new QLabel(this);
    this->label->setGeometry(0,0,665,30);
    this->label->show();
}
void BottomBar::loadstrings(const std::vector<std::wstring>& strlist){
    this->label->hide();
    size_t total=0;
    for(size_t i=0;i<10;i++){
        if(i>=strlist.size())
            break;
        auto ws=strlist.at(i);
        if(total+ws.length()>22)
            break;
        button[i]->setGeometry(30*static_cast<int>(total),0,30*static_cast<int>(ws.length()),30);
        button[i]->setText(QString::fromStdWString(ws));
        button[i]->show();
        this->str[i]=ws;
        total+=ws.length();
    }
}
void BottomBar::loadlabel(const QString text){
    for(size_t i=0;i<10;i++){
        this->button[i]->hide();
    }
    this->label->setText(text);
    this->label->show();
}
