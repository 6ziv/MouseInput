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
    enum{
        INT_EXIT=65535,//the max value of size_t is promised to be no less than 65535.
        INT_ADD_CHARSET=65534,
        INT_REMOVE_CHARSET=65533,
        INT_SWITCH_CHARSET=65532,
        INT_DICTIONARY=65531,
        INT_COPY=65530,
        INT_CUT=65529,
        INT_PASTE=65528
        };
};

#endif // CCHAR_H
