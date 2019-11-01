#pragma once

#include <string>
#include <map>

enum class EKeyState {
    Pressed = 0,
    Held,
    Released
};

struct KeyboardKey {
    int m_ID;
    std::string m_Name;
    EKeyState m_State;
};

class Keyboard {

public:

    void Init();

    void ProcessKeyEvent(int keyID, int keyState);

private:

    std::map<int, KeyboardKey*> m_Keys;

};