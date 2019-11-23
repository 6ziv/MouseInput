#include "sendchar.h"
#include <Windows.h>
void SendChar::SendChar(wchar_t c){
    KEYBDINPUT ki;
    INPUT input;

    ki.wVk = 0;
    ki.wScan = c;
    ki.dwFlags = KEYEVENTF_UNICODE;
    ki.time = 0;
    ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;
    input.ki = ki;
    SendInput(1, &input, sizeof(INPUT));

    ki.wVk = 0;
    ki.wScan = c;
    ki.dwFlags = KEYEVENTF_UNICODE|KEYEVENTF_KEYUP;
    ki.time = 0;
    ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;
    input.ki = ki;
    SendInput(1, &input, sizeof(INPUT));
}
void SendChar::SendVK(unsigned char c){
    SendHalfVK(c,false);
    SendHalfVK(c,true);
}
void SendChar::SendHalfVK(unsigned char c,bool up){
    KEYBDINPUT ki;
    INPUT input;

    ki.wVk = c;
    ki.wScan = 0;
    ki.dwFlags = up?KEYEVENTF_KEYUP:0;
    ki.time = 0;
    ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;
    input.ki = ki;
    SendInput(1, &input, sizeof(INPUT));
}
void SendChar::SendStr(const std::wstring& str){
    for(auto ch : str)
        SendChar(ch);
}
void SendChar::SendModifiedKey(uint8_t vk,uint8_t mod){
    if(mod&CTRL)
        SendHalfVK(VK_CONTROL,false);
    if(mod&ALT)
        SendHalfVK(VK_MENU,false);
    if(mod&SHIFT)
        SendHalfVK(VK_LSHIFT,false);
    if(mod&WIN)
        SendHalfVK(VK_LWIN,false);
    KEYBDINPUT ki;
    INPUT input;

    ki.wVk = vk;
    ki.wScan = 0;
    ki.dwFlags = 0;
    ki.time = 0;
    ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;
    input.ki = ki;
    SendInput(1, &input, sizeof(INPUT));

    ki.wVk = vk;
    ki.wScan = 0;
    ki.dwFlags = KEYEVENTF_KEYUP;
    ki.time = 0;
    ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;
    input.ki = ki;
    SendInput(1, &input, sizeof(INPUT));

    if(mod&CTRL)
        SendHalfVK(VK_CONTROL,true);
    if(mod&ALT)
        SendHalfVK(VK_MENU,true);
    if(mod&SHIFT)
        SendHalfVK(VK_LSHIFT,true);
    if(mod&WIN)
        SendHalfVK(VK_LWIN,true);

}
