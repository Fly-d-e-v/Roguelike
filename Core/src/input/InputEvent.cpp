#include "InputEvent.h"

void InputEvent::AddKeyState(std::shared_ptr<KeyState> keyState) {
    _KeyAction.push_back(keyState);
}

void InputEvent::SetEventStateTrigger(EPressedState pressedState) {
    _EventStateTrigger = pressedState;
}

void InputEvent::AttemptTrigger() {
    for (auto keyState : _KeyAction) {
        if (keyState->_State == _EventStateTrigger) {
            Dispatch();
            switch (keyState->_State) {
            case EPressedState::Pressed:
                keyState->_State = EPressedState::Held;
                break;
            case EPressedState::Released:
                keyState->_State = EPressedState::None;
                break;
            }
            return;
        }
    }
}

void AxisEvent::AddKeyState(std::shared_ptr<AxisState> keyState) {
    _AxisBinding.push_back(keyState);
}

void AxisEvent::AttemptTrigger() {
    for (auto axisState : _AxisBinding) {
        Dispatch(axisState->_Value);
    }
}