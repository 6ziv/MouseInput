#ifndef SENDCHAR_H
#define SENDCHAR_H
#include <string>
namespace SendChar
{
    void SendChar(wchar_t c);
    void SendVK(unsigned char c);
    void SendStr(const std::wstring &str);
};

#endif // SENDCHAR_H
