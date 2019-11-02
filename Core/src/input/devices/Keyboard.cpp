
#include "Keyboard.h"

#include "GLFW/glfw3.h"

#include <assert.h>

void Keyboard::Init() {

    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        std::shared_ptr<KeyState> key = std::make_shared<KeyState>();
        key->m_ID = i;
        m_Keys.insert(std::make_pair(i, key));
    }
}

void Keyboard::ProcessKeyEvent(int keyID, int keyState) {

    auto keyPair = m_Keys.find(keyID);
    if (keyPair != m_Keys.end()) {

        std::shared_ptr<KeyState> key = keyPair->second;
        switch (keyState) {
        case GLFW_PRESS:
            key->m_State = EPressedState::Pressed;
            break;
        case GLFW_REPEAT:
            key->m_State = EPressedState::Held;
            break;
        case GLFW_RELEASE:
            key->m_State = EPressedState::Released;
            break;
        }
    }
    else {
        assert(false && "[Error] Attempting to process unknown key event\n");
    }
}

std::shared_ptr<KeyState> Keyboard::GetKeyState(int id) {
    return m_Keys.find(id)->second;
}
