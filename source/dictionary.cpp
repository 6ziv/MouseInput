#include "dictionary.h"
#include <Windows.h>
#include <codecvt>
#include <sstream>
#include "standalone.h"
bool Dictionary::mopen(){
    std::wstringstream reader;
    for(auto str :Standalone::dictionary)
        reader<<str;
    return sopen(reader);
}
bool Dictionary::open(std::wstring filename){
    std::wifstream reader(filename);
    if(!reader.is_open())return false;
    return sopen(reader);
}
bool Dictionary::sopen(std::wistream &reader){
    dict.clear();
    dict2.clear();

    reader.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstring line;
    std::getline(reader,line);
    if(line.compare(L"Dictionary")!=0)return false;
    std::wstring str;
    while(!reader.eof()){
        reader>>str;
        if(reader.eof())break;
        if(str.length()<2)continue;
        if(str.length()==2){
            if(dict2.end()==dict2.find(str.at(0))){
                std::vector<wchar_t> tmp;
                tmp.clear();
                dict2[str.at(0)]=tmp;
            }
            dict2[str.at(0)].push_back(str.at(1));
        }else{
            if(dict.end()==dict.find(str.at(0))){
                std::vector<std::wstring> tmp;
                tmp.clear();
                dict[str.at(0)]=tmp;
            }
            dict[str.at(0)].push_back(str.substr(1));
        }
    }
    return true;
}
const std::vector<std::wstring>& Dictionary::get(wchar_t ch){
    if(dict.end()==dict.find(ch)){
        return null;
    }
    return dict.at(ch);
}

const std::vector<wchar_t>& Dictionary::get2(wchar_t ch){
    if(dict2.end()==dict2.find(ch)){
        return null2;
    }
    return dict2.at(ch);
}
