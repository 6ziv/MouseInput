#include "charsetselector.h"
#include <Windows.h>
CharsetSelector::CharsetSelector(const std::vector<std::wstring> filenames)
{
    auto charset=std::make_unique<Charset>();
    charset->openDefault();
    charsets.push_back(std::move(charset));
    for(auto filename:filenames){
        open(filename);
    }
    current_charset=0;
}
CharsetSelector::CharsetSelector()
{
    auto charset=std::make_unique<Charset>();
    charset->openDefault();
    charsets.push_back(std::move(charset));

    charset=std::make_unique<Charset>();
    charset->mopen();
    charsets.push_back(std::move(charset));
    current_charset=0;
}
bool CharsetSelector::open(const std::wstring &filename){
    auto charset=std::make_unique<Charset>();
    if(charset->open(filename)){
        charsets.push_back(std::move(charset));
        return true;
    }else
        return false;
}
bool CharsetSelector::close(){
    if(current_charset==0)
        return false;
    auto i=charsets.begin();
    std::advance(i,current_charset);
    charsets.erase(i);
    if(current_charset>=charsets.size())
        current_charset=0;
    return true;
}
const Charset* CharsetSelector::get(){
    return charsets.at(current_charset).get();
}
void CharsetSelector::move(){
    if(current_charset<charsets.size()-1){
        current_charset++;
    }else{
        current_charset=0;
    }
}
void CharsetSelector::select(size_t id){
    if(id<charsets.size())
        current_charset=id;
}
size_t CharsetSelector::pos(){
    return current_charset;
}
