#include "InputEvent.h"

void InputEvent::AddKeyState(std::shared_ptr<KeyState> keyState) {
    m_KeyAction.push_back(keyState);
}

void InputEvent::SetEventStateTrigger(EPressedState pressedState) {
    m_EventStateTrigger = pressedState;
}

void InputEvent::AttemptTrigger() {
    for (auto keyState : m_KeyAction) {
        if (keyState->m_State == m_EventStateTrigger) {
            Dispatch();
            switch (keyState->m_State) {
            case EPressedState::Pressed:
                keyState->m_State = EPressedState::Held;
                break;
            case EPressedState::Released:
                keyState->m_State = EPressedState::None;
                break;
            }
            return;
        }
    }
}

void AxisEvent::AddKeyState(std::shared_ptr<AxisState> keyState) {
    m_AxisBinding.push_back(keyState);
}

void AxisEvent::AttemptTrigger() {
    for (auto axisState : m_AxisBinding) {
        Dispatch(axisState->m_Value);
    }
}