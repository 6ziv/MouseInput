#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <fstream>
#include <vector>
#include <map>
class Dictionary
{
public:
    bool open(std::wstring filename);
    const std::vector<std::wstring>& get(wchar_t);
    const std::vector<wchar_t>& get2(wchar_t);
private:
    std::map<wchar_t,std::vector<std::wstring>>dict;
    std::map<wchar_t,std::vector<wchar_t>>dict2;
    std::vector<std::wstring> null;
    std::vector<wchar_t> null2;
};

#endif // DICTIONARY_H
