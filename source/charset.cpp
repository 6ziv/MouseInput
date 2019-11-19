#include "charset.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <Windows.h>
bool charset::open(std::wstring filename){
    Cats.clear();
    Chars.clear();
    std::wifstream input(filename);
    if(!input.is_open()){MessageBoxW(0,filename.c_str(),L"CanNotOpen",MB_OK);return false;}
    input.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstring line;
    std::getline(input,line);
    if(line.compare(L"Charset")!=0)return false;
    while(!input.eof()){
        std::getline(input,line);
        std::wistringstream memin(line);
        std::wstring tag;
        memin>>tag;
        if(memin.eof())continue;
        std::wstring detail;
        memin>>detail;
        if(memin.eof())continue;
        std::vector<wchar_t> ws;
        wchar_t ch;
        do{
            memin>>ch;
            if(ch==L'\r' || ch==L'\n')break;
            ws.push_back(ch);
        }while(!memin.eof());

        std::pair<std::pair<std::wstring,std::wstring>,size_t> tmp;
        tmp.first.first=tag;
        tmp.first.second=detail;
        tmp.second=Chars.size();
        Chars.push_back(ws);
        Cats.push_back(tmp);
    }
    return true;
}
void charset::openDefault(){
    const wchar_t str[]=L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Cats.clear();
    Chars.clear();
    for(size_t i=0;i<52;i++){
        std::pair<std::pair<std::wstring,std::wstring>,size_t> tmp;
        tmp.first.first.clear();
        tmp.first.first.push_back(str[i]);
        tmp.first.second=tmp.first.first;
        tmp.second=i;
        Chars.push_back(std::vector<wchar_t>());
        Chars[i].push_back(str[i]);
        Cats.push_back(tmp);
    }
}
size_t charset::CountCatalogs(){
    return Cats.size();
}
const std::wstring& charset::GetCatalogTag(size_t catalog){
    return Cats[catalog].first.first;
}
const std::wstring& charset::GetCatalogDescription(size_t catalog){
    return Cats[catalog].first.second;
}
const std::vector<wchar_t>& charset::GetCatalog(size_t catalog){
    return Chars.at(Cats[catalog].second);
}
