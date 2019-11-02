#pragma once

#include "input/utils/InputEnums.h"
#include "input/utils/InputTypes.h"

#include <string>
#include <map>
#include <memory>

class Keyboard {

public:

    void Init();
    void ProcessKeyEvent(int keyID, int keyState);

    std::shared_ptr<KeyState> GetKeyState(int id);

private:

    std::map<int, std::shared_ptr<KeyState>> m_Keys;

};