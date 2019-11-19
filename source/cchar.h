#ifndef CCHAR_H
#define CCHAR_H
#include <string>
class CChar
{
public:
    enum{
        CChar_Invalid=0,
        CChar_Char,
        CChar_VK,
        CChar_Catalog,
        CChar_Internal
    } type=CChar_Invalid;
    union{
        wchar_t ch;
        unsigned char vk;
        size_t cid;
        size_t cmd;
    } ch;
};

#endif // CCHAR_H
