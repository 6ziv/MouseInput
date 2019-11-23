#ifndef SENDCHAR_H
#define SENDCHAR_H
#include <string>
namespace SendChar
{
    enum{
        CTRL=1,
        ALT=2,
        SHIFT=4,
        WIN=8
    };
    void SendChar(wchar_t c);
    void SendVK(unsigned char c);
    void SendStr(const std::wstring &str);
    void SendModifiedKey(uint8_t vk,uint8_t mod);
    void SendHalfVK(unsigned char c,bool up);
};

#endif // SENDCHAR_H
