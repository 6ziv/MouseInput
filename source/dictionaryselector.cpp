#include "dictionaryselector.h"
#include <QLabel>
#include <QDir>
#include <fstream>
#include <codecvt>
#include <QFileDialog>
#include <QMessageBox>
#include "dictionary.h"
DictionarySelector::DictionarySelector(QApplication *app,QSettings *settings)
    :QDialog(nullptr)
{
    this->setWindowTitle(QString::fromWCharArray(L"选择字典"));
    application=app;
    std::wstring tags[]={L"IT",L"财经",L"成语",L"地名",L"历史名人",L"诗词",L"医学",L"饮食",L"法律",L"汽车",L"动物"};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            if(i==3&&j==2)continue;
            cb[i*3+j]=new QCheckBox(this);
            cb[i*3+j]->setText(QString::fromStdWString(tags[i*3+j]));
            cb[i*3+j]->setGeometry(10+80*j,10+30*i,80,20);
            cb[i*3+j]->setChecked(true);
            cb[i*3+j]->show();
        }
    }
    this->resize(260,300);
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());
    QDir::setCurrent(QApplication::applicationDirPath());
    std::wstring names[]={L"THUOCL_it",L"THUOCL_caijing",L"THUOCL_chengyu",L"THUOCL_diming",L"THUOCL_lishimingren",L"THUOCL_poem",L"THUOCL_medical",L"THUOCL_food",L"THUOCL_law",L"THUOCL_car",L"THUOCL_animal"};

    QLabel* lab=new QLabel(this);
    lab->setGeometry(5,180,250,20);
    lab->show();

    build=new QPushButton(this);
    build->setGeometry(30,200,80,30);
    build->setText(QString::fromWCharArray(L"构建词典"));
    build->show();

    connect(build,&QPushButton::pressed,this,[=](){
            std::wstring out;
            std::map<std::wstring,size_t> map;
            for(int i=0;i<11;i++){
                if(cb[i]->isChecked()){
                    std::wstring filename=std::wstring(L"data/")+names[i]+L".txt";
                    std::wifstream reader(filename);
                    if(!reader.is_open()){
                        continue;
                    }
                    reader.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
                    std::wstring ws;
                    size_t f;
                    while(!reader.eof()){
                        reader>>ws;
                        if(reader.eof())break;
                        if(map.find(ws)==map.end())map[ws]=0;
                        reader>>f;
                        map[ws]+=f;
                        lab->setText(QString::fromStdWString(filename)+":"+QString::fromStdWString(ws));
                        application->processEvents();
                    }
                }
            }
            lab->setText(QString::fromWCharArray(L"正在处理数据"));
            application->processEvents();
            std::vector<std::pair<std::wstring,size_t>> vec;
            vec.assign(map.begin(),map.end());
            std::sort(vec.begin(),vec.end(),[](
                      std::pair<std::wstring,size_t> a,
                      std::pair<std::wstring,size_t> b
                      )->bool
            {
                return a.second>b.second;
            });
            application->processEvents();

            std::map<wchar_t,size_t> count1;
            std::map<wchar_t,size_t> count2;

            std::wofstream output("generated.dct");
            output.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
            if(!output.is_open()){
                lab->setText(QString::fromWCharArray(L"无法输出文件"));
                return;
            }
            lab->setText(QString::fromWCharArray(L"正在写入。。。"));
            application->processEvents();
            output<<"Dictionary"<<std::endl;
            for(auto i:vec){
                std::wstring str=i.first;
                if(str.length()==2){
                    if(count1.find(str.at(0))==count1.end())count1[str.at(0)]=0;
                    count1[str.at(0)]++;
                    if(count1[str.at(0)]<65){
                        output<<str+L" ";
                    }
                }else{
                    if(count2.find(str.at(0))==count2.end())count2[str.at(0)]=0;
                    count2[str.at(0)]++;
                    if(count2[str.at(0)]<11){
                        output<<str+L" ";
                    }
                }
            }
            lab->setText(QString::fromWCharArray(L"完成"));
            settings->sync();
            settings->setValue("dictionary","generated.dct");
            settings->sync();
            application->processEvents();
            this->accept();
        }
    );

    select=new QPushButton(this);
    select->setGeometry(150,200,80,30);
    select->show();
    select->setText(QString::fromWCharArray(L"从文件导入"));
    connect(select,&QPushButton::pressed,this,[=](){
        QString path=QFileDialog::getOpenFileName(this,"Select dictionary",QApplication::applicationDirPath(),"Dictionary (*.dct);;Text files (*.txt);;All files (*.*)");
        if(path.isEmpty())return;
        Dictionary dict;
        if(!dict.open(path.toStdWString())){
            QMessageBox(QMessageBox::Icon::Warning,"Error","Can not open dictionary",QMessageBox::Ok,this).exec();
            return;
        }
        settings->setValue("dictionary",path);
        settings->sync();
        this->accept();
    });
}
