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
    KEYBDINPUT ki;
    INPUT input;

    ki.wVk = c;
    ki.wScan = 0;
    ki.dwFlags = 0;
    ki.time = 0;
    ki.dwExtraInfo = 0;
    input.type = INPUT_KEYBOARD;
    input.ki = ki;
    SendInput(1, &input, sizeof(INPUT));

    ki.wVk = c;
    ki.wScan = 0;
    ki.dwFlags = KEYEVENTF_KEYUP;
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
