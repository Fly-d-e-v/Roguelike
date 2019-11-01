#include "Keyboard.h"

#include "GLFW/glfw3.h"

#include <assert.h>

void Keyboard::Init() {

    for (int i = 0; i < GLFW_KEY_LAST; i++) {
        const char * keyName = glfwGetKeyName(i, 0);
        if (keyName == nullptr) 
            continue;

        std::string sKeyName = keyName;
        if (sKeyName != "") {
            KeyboardKey* key = new KeyboardKey();
            key->m_ID = i;
            key->m_Name = keyName;
            m_Keys.insert(std::make_pair(i, key));
        }
    }
}

void Keyboard::ProcessKeyEvent(int keyID, int keyState) {

    auto keyPair = m_Keys.find(keyID);
    if (keyPair != m_Keys.end()) {

        KeyboardKey* key = keyPair->second;
        switch (keyState) {
        case GLFW_PRESS:
            key->m_State = EKeyState::Pressed;
            break;
        case GLFW_REPEAT:
            key->m_State = EKeyState::Held;
            break;
        case GLFW_RELEASE:
            key->m_State = EKeyState::Released;
            break;
        }
    }
    else {
        assert("[Error] Attempting to process unknown key event\n");
    }
}
