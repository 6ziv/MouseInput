#include "config.h"
#include "catpanel.h"
#include "mainwindow.h"
#include <QFont>
#include <Windows.h>
CatPanel::CatPanel(MainWindow *m,QFont &font)
    :QWidget(m)
{
    main=m;
    this->resize(420,240);
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            button[i][j]=new Button();
            button[i][j]->setParent(this);
            button[i][j]->setGeometry(i*35,j*20,35,20);
            button[i][j]->setFont(font);
            connect(button[i][j],&Button::pressed,this,[=](){
                main->catpressed(Char[i][j]);
            });
            connect(button[i][j],&Button::mouseenter,this,[=](){
                main->catpreview(Char[i][j]);
            });

            button[i][j]->show();
        }
    }

    this->SetVK(10,0,"BS",VK_BACK);
    this->SetVK(10,1,"UP",VK_UP);
    this->SetVK(10,2,"LEFT",VK_LEFT);
    this->SetVK(10,3,"RIGHT",VK_RIGHT);
    this->SetVK(10,4,"DOWN",VK_DOWN);
    this->SetVK(10,5,"SPACE",VK_SPACE);
    this->SetVK(10,6,"TAB",VK_TAB);
    this->SetVK(10,7,"HOME",VK_HOME);
    this->SetVK(10,8,"END",VK_END);
    this->SetVK(10,9,"DEL",VK_DELETE);
    this->SetVK(10,10,"ESC",VK_ESCAPE);
    this->SetVK(10,11,"ENTER",VK_RETURN);
    this->SetVK(11,1,QString::fromStdWString(L"上页"),VK_PRIOR);
    this->SetVK(11,2,QString::fromStdWString(L"下页"),VK_NEXT);
    this->SetIntenal(11,3,"CTR+C",CChar::INT_COPY);
    this->SetIntenal(11,4,"CTR+X",CChar::INT_CUT);
    this->SetIntenal(11,5,"CTR+V",CChar::INT_PASTE);
    //this->SetVK(11,5,QString::fromStdWString(L"WIN"),VK_LWIN);
#ifndef STANDALONE
    this->SetIntenal(11,8,QString::fromStdWString(L"词典"),CChar::INT_DICTIONARY);
    this->SetIntenal(11,9,QString::fromStdWString(L"字典"),CChar::INT_ADD_CHARSET);
    this->SetIntenal(11,10,QString::fromStdWString(L"移除"),CChar::INT_REMOVE_CHARSET);
#endif
    this->SetIntenal(11,11,QString::fromStdWString(L"切换"),CChar::INT_SWITCH_CHARSET);
    this->SetIntenal(11,0,QString::fromStdWString(L"退出"),CChar::INT_EXIT);
}
void CatPanel::SetVK(int x,int y,const QString tag,unsigned char VK){
    QString tag1=tag;
    button[x][y]->setText(tag1.replace('&',"&&"));
    Char[x][y].type=CChar::CChar_VK;
    Char[x][y].ch.vk=VK;
}
void CatPanel::SetChar(int x,int y,const QString tag,wchar_t wc){
    QString tag1=tag;
    button[x][y]->setText(tag1.replace('&',"&&"));
    Char[x][y].type=CChar::CChar_Char;
    Char[x][y].ch.ch=wc;
}
void CatPanel::SetCatalog(int x,int y,const QString tag,size_t cid){
    QString tag1=tag;
    button[x][y]->setText(tag1.replace('&',"&&"));
    Char[x][y].type=CChar::CChar_Catalog;
    Char[x][y].ch.cid=cid;
}
void CatPanel::SetIntenal(int x,int y,const QString tag,size_t cmd){
    QString tag1=tag;
    button[x][y]->setText(tag1.replace('&',"&&"));
    Char[x][y].type=CChar::CChar_Internal;
    Char[x][y].ch.cmd=cmd;
}
void CatPanel::load(const Charset* cs){
    for(int i=0;i<10;i++){
        for(int j=0;j<12;j++){
            size_t id=static_cast<size_t>(12*i+j);
            if(cs->CountCatalogs()>id){
                if(cs->GetCatalog(id).size()==1){
                    this->SetChar(
                            i,
                            j,
                            QString::fromStdWString(cs->GetCatalogTag(id)),
                            cs->GetCatalog(id).at(0)
                        );
                }else{
                    this->SetCatalog(
                            i,
                            j,
                            QString::fromStdWString(cs->GetCatalogTag(id)),
                            id
                        );
                }
            }else{
                button[i][j]->setText("");
                Char[i][j].type=CChar::CChar_Invalid;
            }
        }
    }
}
