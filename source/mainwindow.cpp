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
#include "dictionaryselector.h"
MainWindow::MainWindow(QWidget *parent,QApplication *app)
    : QWidget(parent)
{
    application=app;
    settings=std::make_unique<QSettings>(QApplication::applicationDirPath()+"/settings.ini",QSettings::IniFormat);
    dictionary=std::make_unique<Dictionary>();
    if(settings->contains("dictionary")){
        dictionary->open(settings->value("dictionary","").toString().toStdWString());
    }

    charsets.push_back(std::make_unique<charset>());
    charsets.at(0)->openDefault();

    if(settings->contains("charset")){
        auto cslist=settings->value("charset","").toStringList();
        for(auto i=cslist.begin();i!=cslist.end();i++){
            charsets.push_back(std::make_unique<charset>());
            if(!charsets.at(charsets.size()-1)->open(i->toStdWString())){
                charsets.pop_back();
            }
        }
    }
    current_charset=charsets.begin();
    if(settings->contains("fav_charset")){
        bool ok;
        auto ccs=settings->value("fav_charset",0).toUInt(&ok);
        if(ok && ccs<charsets.size())
            std::advance(current_charset,ccs);
    }
    auto favpos=settings->value("fav_position",QPoint(QApplication::screens().at(0)->size().width()-675,QApplication::screens().at(0)->size().height()-300)).toPoint();

    this->setMouseTracking(true);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::WindowDoesNotAcceptFocus);
    this->setWindowOpacity(0.5);
    this->setGeometry(favpos.x(),favpos.y(),675,280);
    SetWindowLong(HWND(this->winId()),GWL_EXSTYLE,GetWindowLong(HWND(this->winId()),GWL_EXSTYLE)|WS_EX_TOPMOST | WS_EX_NOACTIVATE);
    QFont font;
    font.setPixelSize(10);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            btnChar[i][j]=new QPushButton(this);
            btnChar[i][j]->setGeometry(i*30+5,j*30+5,30,30);
            btnChar[i][j]->show();
            connect(btnChar[i][j],&QPushButton::pressed,this,[=](){
                if(CharPad[i][j].type==CChar::CChar_Char){
                    SendChar::SendChar(CharPad[i][j].ch.ch);
                    ShowImagine(CharPad[i][j].ch.ch);
                    return;
                }
                if(CharPad[i][j].type==CChar::CChar_Internal){
                    ShowCat(current_cid,static_cast<int>(CharPad[i][j].ch.cmd));
                    return;
                }
            });
        }
    }

    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            btnCatalog[i][j]=new PushButton(this,i,j);
            btnCatalog[i][j]->setGeometry(250+i*35,j*20+5,35,20);
            btnCatalog[i][j]->setFont(font);
            btnCatalog[i][j]->show();
            connect(btnCatalog[i][j],&PushButton::pressed,this,[=](){catpressed(i,j);});
            if((*current_charset)->CountCatalogs()>12*i+j){
                if((*current_charset)->GetCatalog(12*i+j).size()==1){
                    this->SetChar(i,j,QString::fromStdWString((*current_charset)->GetCatalogTag(12*i+j)),(*current_charset)->GetCatalog(12*i+j).at(0));
                }else{
                    this->SetCatalog(i,j,QString::fromStdWString((*current_charset)->GetCatalogTag(12*i+j)),12*i+j);
                }
            }
        }
    }
    for(int i=0;i<10;i++){
        btnImagine[i]=new QPushButton(this);
        btnImagine[i]->hide();
        connect(btnImagine[i],&PushButton::pressed,this,[=](){ImaginePressed(i);});
    }
    LongTab=new QLabel(this);
    LongTab->setGeometry(5,250,665,30);
    LongTab->show();
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
    this->SetIntenal(11,0,QString::fromStdWString(L"退出"),255);
    this->SetVK(11,1,QString::fromStdWString(L"上页"),VK_PRIOR);
    this->SetVK(11,2,QString::fromStdWString(L"下页"),VK_NEXT);
    this->SetVK(11,5,QString::fromStdWString(L"WIN"),VK_LWIN);

    this->SetIntenal(11,8,QString::fromStdWString(L"词典"),252);
    this->SetIntenal(11,9,QString::fromStdWString(L"字典"),253);
    this->SetIntenal(11,10,QString::fromStdWString(L"移除"),251);
    this->SetIntenal(11,11,QString::fromStdWString(L"切换"),254);
    this->show();
}
void MainWindow::SetVK(int x,int y,QString tag,unsigned char VK){
    btnCatalog[x][y]->setText(tag);
    CatChar[x][y].type=CChar::CChar_VK;
    CatChar[x][y].ch.vk=VK;
}
void MainWindow::SetChar(int x,int y,QString tag,wchar_t c){
    btnCatalog[x][y]->setText(tag);
    CatChar[x][y].type=CChar::CChar_Char;
    CatChar[x][y].ch.ch=c;
}
void MainWindow::SetCatalog(int x,int y,QString tag,size_t cid){
    btnCatalog[x][y]->setText(tag);
    CatChar[x][y].type=CChar::CChar_Catalog;
    CatChar[x][y].ch.cid=cid;
}
void MainWindow::SetIntenal(int x,int y,QString tag,unsigned char cmd){
    btnCatalog[x][y]->setText(tag);
    CatChar[x][y].type=CChar::CChar_Internal;
    CatChar[x][y].ch.cmd=cmd;
}
void MainWindow::enterEvent(QEvent *){
    this->setWindowOpacity(1);
}
void MainWindow::leaveEvent(QEvent *){
    this->setWindowOpacity(0.4);
    ShowCat(-1);
}
MainWindow::~MainWindow()
{
}
bool MainWindow::nativeEvent(const QByteArray &, void *message, long *r){
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
void MainWindow::mousePressEvent(QMouseEvent *e){
    if(e->button()!=Qt::LeftButton)return;
    IsDragging=true;
    pt=e->pos();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *){
    IsDragging=false;
    QPoint p=this->pos();
    settings->sync();
    settings->setValue("fav_position",p);
    settings->sync();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e){
    if(!e->buttons().testFlag(Qt::MouseButton::LeftButton)){
        IsDragging=false;
        return;
    }
    if(!IsDragging)
        return;
    this->move(this->pos()+e->pos()-pt);
}
void MainWindow::catpreview(int x,int y){
    if(CatChar[x][y].type!=CChar::CChar_Catalog){
        ShowCat(-1);
    }else{
        ShowCat(static_cast<int>(CatChar[x][y].ch.cid));
    }
}

void MainWindow::catpressed(int x,int y){
    catpreview(x,y);
    if(CatChar[x][y].type==CChar::CChar_VK){
        SendChar::SendVK(CatChar[x][y].ch.vk);
    }
    if(CatChar[x][y].type==CChar::CChar_Char){
        SendChar::SendChar(CatChar[x][y].ch.ch);
    }
    if(CatChar[x][y].type==CChar::CChar_Catalog){
        GoToBtn(btnChar[3][3]);
    }
    if(CatChar[x][y].type==CChar::CChar_Internal){
        if(CatChar[x][y].ch.cmd==255)ExitProcess(0);
        if(CatChar[x][y].ch.cmd==254){
            current_charset++;
            if(current_charset==charsets.end())
                current_charset=charsets.begin();
            auto t=std::distance(charsets.begin(),current_charset);
            settings->setValue("fav_charset",t);
            settings->sync();
            LoadCats();
            ShowCat(-1);
        }
        if(CatChar[x][y].ch.cmd==253){
            QString path=QFileDialog::getOpenFileName(this,"Select charset",QApplication::applicationDirPath(),"Charset (*.cst);;Text files (*.txt);;All files (*.*)");
            if(path.isEmpty())return;
            auto t=std::distance(charsets.begin(),current_charset);
            charsets.push_back(std::make_unique<charset>());
            if(!charsets.at(charsets.size()-1)->open(path.toStdWString())){
                charsets.pop_back();
                QMessageBox(QMessageBox::Icon::Warning,"Error","Can not open charset",QMessageBox::Ok,this).exec();
            }else{
                settings->sync();
                QStringList sl=settings->value("charset","").toStringList();
                if(sl.contains(path)){
                    charsets.pop_back();
                    QMessageBox(QMessageBox::Icon::Warning,"Error","Duplicate charsets",QMessageBox::Ok,this).exec();
                }else{
                    sl.push_back(path);
                    settings->setValue("charset",sl);
                    settings->sync();
                }
            }
            current_charset=charsets.begin();
            std::advance(current_charset,t);
        }
        if(CatChar[x][y].ch.cmd==252){
            DictionarySelector ds(application,settings.get());
            if(QDialog::Accepted == ds.exec()){
                QString p=settings->value("dictionary","").toString();
                if(p.isNull() ||p.isEmpty())return;
                auto t=std::make_unique<Dictionary>();
                if(t->open(p.toStdWString())){
                    dictionary=std::move(t);
                }else{
                    QMessageBox(QMessageBox::Icon::Warning,"Error","Can not open dictionary",QMessageBox::Ok,this).exec();
                }
            }
        }
        if(CatChar[x][y].ch.cmd==251){
            if(current_charset==charsets.begin()){
                QMessageBox(QMessageBox::Icon::Warning,"Error",QString::fromStdWString(L"不能移除默认字库"),QMessageBox::Ok,this).exec();
                return;
            }
            auto t=std::distance(charsets.begin(),current_charset);
            charsets.erase(current_charset);
            auto t1=t<charsets.size()?t:0;
            current_charset=charsets.begin();
            std::advance(current_charset,t1);
            settings->sync();
            QStringList sl=settings->value("charset","").toStringList();
            sl.removeAt(t);
            settings->setValue("charset",sl);
            settings->sync();
            LoadCats();
            ShowCat(-1);
        }
    }
}
void MainWindow::ImaginePressed(int id){
    for(auto t=im[id].cbegin();t!=im[id].cend();t++)
        SendChar::SendChar(*t);
}
void MainWindow::ShowCat(int cid,int page){
    current_cid=cid;
    LongTab->show();
    for(int i=0;i<10;i++){
        btnImagine[i]->hide();
    }
    if(cid<0){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                CharPad[i][j].type=CChar::CChar_Invalid;
                btnChar[i][j]->setText("");
            }
        }
        LongTab->setText("");
    }else{
        LongTab->setText(QString::fromStdWString((*current_charset)->GetCatalogDescription(static_cast<size_t>(cid))));
        if((*current_charset)->GetCatalog(static_cast<size_t>(cid)).size()<=64){
            for(size_t i=0;i<8;i++){
                for(size_t j=0;j<8;j++){
                    if(i*8+j>=(*current_charset)->GetCatalog(static_cast<size_t>(cid)).size()){
                        CharPad[j][i].type=CChar::CChar_Invalid;
                        btnChar[j][i]->setText("");
                    }else{
                        CharPad[j][i].type=CChar::CChar_Char;
                        CharPad[j][i].ch.ch=(*current_charset)->GetCatalog(static_cast<size_t>(cid)).at(i*8+j);
                        btnChar[j][i]->setText(QString::fromStdWString(std::wstring()+CharPad[j][i].ch.ch));
                    }
                }
            }
        }else{
            size_t totalpages=((*current_charset)->GetCatalog(static_cast<size_t>(cid)).size()-1)/60+1;
            CharPad[2][7].type=CChar::CChar_Internal;
            CharPad[2][7].ch.cmd=0;
            btnChar[2][7]->setText(QString::fromStdWString(L"首页"));
            CharPad[3][7].type=CChar::CChar_Internal;
            CharPad[3][7].ch.cmd=(page==0)?0:page-1;
            btnChar[3][7]->setText(QString::fromStdWString(L"上页"));
            CharPad[4][7].type=CChar::CChar_Internal;
            CharPad[4][7].ch.cmd=(page==totalpages-1)?totalpages-1:page+1;
            btnChar[4][7]->setText(QString::fromStdWString(L"下页"));
            CharPad[5][7].type=CChar::CChar_Internal;
            CharPad[5][7].ch.cmd=totalpages-1;
            btnChar[5][7]->setText(QString::fromStdWString(L"末页"));
            for(size_t i=0;i<8;i++){
                for(size_t j=0;j<8;j++){
                    size_t tmpj;
                    if(i==7 && j>5)tmpj=j-4;
                    else tmpj=j;
                    if(i==7 && j<6 && j>1){
                        continue;
                    }
                    if(i*8+tmpj+page*60>=(*current_charset)->GetCatalog(static_cast<size_t>(cid)).size()){
                        CharPad[j][i].type=CChar::CChar_Invalid;
                        btnChar[j][i]->setText("");
                    }else{
                        CharPad[j][i].type=CChar::CChar_Char;
                        CharPad[j][i].ch.ch=(*current_charset)->GetCatalog(static_cast<size_t>(cid)).at(i*8+tmpj+page*60);
                        btnChar[j][i]->setText(QString::fromStdWString(std::wstring()+CharPad[j][i].ch.ch));
                    }
                }
            }
        }
    }
}

void MainWindow::ShowImagine(wchar_t ch){
    for(size_t i=0;i<8;i++){
        for(size_t j=0;j<8;j++){
            if(i*8+j>=dictionary->get2(ch).size()){
                CharPad[j][i].type=CChar::CChar_Invalid;
                btnChar[j][i]->setText("");
            }else{
                CharPad[j][i].type=CChar::CChar_Char;
                CharPad[j][i].ch.ch=dictionary->get2(ch).at(i*8+j);
                btnChar[j][i]->setText(QString::fromStdWString(std::wstring()+CharPad[j][i].ch.ch));
            }
        }
    }
    size_t totallength=0;
    LongTab->hide();
    for(size_t longl=0;longl<10;longl++){
        if(longl>=dictionary->get(ch).size())break;
        auto ws=dictionary->get(ch).at(longl);
        if(totallength+ws.length()>22)break;
        btnImagine[longl]->setGeometry(5+30*totallength,250,30*ws.length(),30);
        btnImagine[longl]->setText(QString::fromStdWString(ws.c_str()));
        btnImagine[longl]->show();

        im[longl]=ws;
        totallength+=ws.length();

    }
}
void MainWindow::GoToBtn(QPushButton *pb){
    if(pb->parent()!=this)return;
    auto p=pb->geometry().center()+this->pos();
    SetCursorPos(p.x(),p.y());
}
void MainWindow::LoadCats(){
    for(int i=0;i<10;i++){
        for(int j=0;j<12;j++){
            if((*current_charset)->CountCatalogs()>12*i+j){
                if((*current_charset)->GetCatalog(12*i+j).size()==1){
                    this->SetChar(i,j,QString::fromStdWString((*current_charset)->GetCatalogTag(12*i+j)),(*current_charset)->GetCatalog(12*i+j).at(0));
                }else{
                    this->SetCatalog(i,j,QString::fromStdWString((*current_charset)->GetCatalogTag(12*i+j)),12*i+j);
                }
            }else{
                btnCatalog[i][j]->setText("");
                CatChar[i][j].type=CChar::CChar_Invalid;
            }
        }
    }
}
