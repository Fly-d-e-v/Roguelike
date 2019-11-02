
#include "Keyboard.h"

#include "GLFW/glfw3.h"

#include <assert.h>

void Keyboard::Init() {

    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        std::shared_ptr<KeyState> key = std::make_shared<KeyState>();
        key->m_ID = i;
        _Keys.insert(std::make_pair(i, key));
    }
}

void Keyboard::ProcessKeyEvent(int keyID, int keyState) {

    auto keyPair = _Keys.find(keyID);
    if (keyPair != _Keys.end()) {

        std::shared_ptr<KeyState> key = keyPair->second;
        switch (keyState) {
        case GLFW_PRESS:
            key->_State = EPressedState::Pressed;
            break;
        case GLFW_REPEAT:
            key->_State = EPressedState::Held;
            break;
        case GLFW_RELEASE:
            key->_State = EPressedState::Released;
            break;
        }
    }
    else {
        assert(false && "[Error] Attempting to process unknown key event\n");
    }
}

std::shared_ptr<KeyState> Keyboard::GetKeyState(int id) {
    return _Keys.find(id)->second;
}
